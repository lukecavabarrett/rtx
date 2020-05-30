#include "rtx.h"
#include "camera.h"

#include <cstdlib>
#include <cmath>
#include <numeric>
#include <thread>
#include <cassert>
#include <random>
#include <queue>
#include <future>
namespace rtx {
camera::camera(const vector3 &p, const vector3 &d, const vector3 &v, int w, int h, dtype f, int b) {
  position = p;
  depth = d.normalised();
  vertical = v.normalised();
  width_px = w;
  height_px = h;
  fov = f;
  bounces = b;
  aspect_ratio = ((dtype) width_px) / ((dtype) height_px);
  width_m = std::tan(fov * M_PIl / 360) * 2;
  height_m = width_m / aspect_ratio;
  x_step_m = width_m / width_px;
  y_step_m = height_m / height_px;
}
ray3 camera::cast_ray(int x, int y) {
  dtype x_pos = (x_step_m - width_m) / 2 + x * x_step_m;
  dtype y_pos = (y_step_m + height_m) / 2 - y * y_step_m;
  return ray3(position, (depth + vertical * y_pos + depth.cross(vertical) * x_pos - position).normalised());
}
ray3 camera::cast_ray(dtype x, dtype y) {
  dtype x_pos = (x_step_m - width_m) / 2 + x * x_step_m;
  dtype y_pos = (y_step_m + height_m) / 2 - y * y_step_m;
  return ray3(position, (depth + vertical * y_pos + depth.cross(vertical) * x_pos - position).normalised());
}
color_rgb camera::get_pixel(int x, int y, const scene &s) {
  return s.trace(cast_ray(x, y), bounces);
}

color_rgb camera::get_pixel(double x, double y, const scene &s) {
  return s.trace(cast_ray(x, y), bounces);
}
void camera::render_screen(screen &scr, const scene &s) {
#pragma omp parallel for
  for (int y = 0; y < height_px; y++) {
    for (int x = 0; x < width_px; x++) {

      scr.put_pixel(x, y, get_pixel(x, y, s).tonemap().to_rgb_le());
      //*(int *)i = ((x>200)?(color_rgb<D>::from_rgb(0x333333)):(color_rgb<D>(0))).tonemap().to_rgb_le();
    }
    //if(y%10==0 || y==height_px-1)fprintf(stderr,"\b\b\b\b\b\b\b\b\b\b\b\b%2.2f%%",((dtype)y+1)*100/height_px);
  }
  fprintf(stderr, "\n");
}
void camera::render_screen_adaptive(screen &scr, const scene &s) {
  static std::random_device rd;  //Will be used to obtain a seed for the random number engine
  static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  static std::uniform_real_distribution<> dis(0.0, 1.0);

  int square_size = 1;
  while ((height_px % (square_size << 1) == 0) && (width_px % (square_size << 1) == 0))square_size <<= 1;
  while (square_size) {
    int rendered_points = 0;
    for (int y = 0; y < height_px; y += square_size) {
      for (int x = 0; x < width_px; x += square_size) {
        auto c = get_pixel(x, y, s).tonemap().to_rgb_le();
        rendered_points++;
        for (int yp = y; yp < std::min(y + square_size, height_px); ++yp)
          for (int xp = x; xp < std::min(x + square_size, width_px); ++xp)
            scr.put_pixel(xp, yp, c);
        //*(int *)i = ((x>200)?(color_rgb<D>::from_rgb(0x333333)):(color_rgb<D>(0))).tonemap().to_rgb_le();
      }
    }
    assert(((width_px + square_size - 1) / square_size) * ((height_px + square_size - 1) / square_size) == rendered_points);
    fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b%dx%d (%d)", (width_px + square_size - 1) / square_size, (height_px + square_size - 1) / square_size, square_size);

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    square_size >>= 1;
  }
  fprintf(stderr, "\n");

}

namespace adt {
constexpr size_t pixel_size = 16;
struct square {
  double score;
  size_t x, y, size; // these are expressed in pixel_size coordinates
  size_t sx, sy; // the sampled point
  color_rgb color;
  bool operator<(const square &o) const { return score < o.score; }
  void draw(screen &scr, bool wire = false) const {
    assert(size >= pixel_size);
    //draw
    scr.put_rectangle(x / pixel_size, y / pixel_size, size / pixel_size, size / pixel_size, color.tonemap().to_rgb_le());
    //scr.put_pixel(x/pixel_size,y/pixel_size,color.tonemap().to_rgb_le());
    //bounding box
    if (wire)scr.put_rectangle(x / pixel_size, y / pixel_size, size / pixel_size, size / pixel_size, 0xffd300, false);

  }

  bool splittable() const { return size > pixel_size; }
};
template<typename _Callable, typename OutIt>
void split(square source,std::mt19937 &gen, _Callable getp, OutIt begin, OutIt end) {
  assert(source.size > pixel_size);
  bool plus_y = false, plus_x = false;
  vector3 mean = 0;
  for (auto it=begin;it!=end;++it) {
    square &s = *it;
    s = source;
    s.size >>= 1;
    if (plus_x)s.x += s.size;
    if (plus_y)s.y += s.size;
    if (s.sx < s.x || s.sx >= s.x + s.size || s.sy < s.y || s.sy >= s.y + s.size) {
      //regenerate
      s.sx = s.x + std::uniform_int_distribution<>(0, s.size - 1)(gen);
      s.sy = s.y + std::uniform_int_distribution<>(0, s.size - 1)(gen);
      s.color = getp(s.sx, s.sy);
    }
    mean += s.color;
    s.score = 0;
    if (plus_x)plus_y ^= 1;
    plus_x ^= 1;
  }
  mean /= 4;
  double variance = 0;
  for (auto it=begin;it!=end;++it)variance += mean.distance(it->color);
  variance /= 4;
  //std::cerr << "variance is " << variance << std::endl;
  constexpr double eps = 0.0001;
  for (auto it=begin;it!=end;++it)it->score = (variance + eps) * it->size * it->size;
}
}

template<typename R>
bool is_ready(std::future<R> const &f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

std::tuple<std::promise<void>, std::future<size_t>> camera::render_screen_2(screen &scr, const scene &s, bool wire, size_t rays_limit) {
  //static std::random_device rd;  //Will be used to obtain a seed for the random number engine
  static std::mt19937 gen(1234); //Standard mersenne_twister_engine seeded with rd()
  //static std::uniform_real_distribution<> dis(0.0, 1.0);

  std::promise<void> p;
  std::future<size_t> f = std::async([this, &scr, &s, wire, fut = p.get_future()]() {
    size_t square_size = 1;
    size_t r = 0;
    while ((height_px % (square_size << 1) == 0) && (width_px % (square_size << 1) == 0))square_size <<= 1;
    std::priority_queue<adt::square> pq;
    for (size_t y = 0; y < height_px; y += square_size)
      for (size_t x = 0; x < width_px; x += square_size) {
        ++r;
        size_t sx = x * adt::pixel_size + std::uniform_int_distribution<>(0, square_size * adt::pixel_size - 1)(gen);
        size_t sy = y * adt::pixel_size + std::uniform_int_distribution<>(0, square_size * adt::pixel_size - 1)(gen);
        color_rgb c = get_pixel(double(sx) / adt::pixel_size, double(sy) / adt::pixel_size, s);
        adt::square s = {.score = 1000000000, .x=x * adt::pixel_size, .y=y * adt::pixel_size, .size=square_size * adt::pixel_size, .sx=sx, .sy=sy, .color=c};
        s.draw(scr, wire);
        pq.push(s);
      }
    constexpr int thread_num = 1;
    adt::square outs[thread_num*4];
    std::future<void> trs[thread_num];
    while ( !pq.empty() ) {
      if (is_ready(fut)) {
        std::cerr << "early exit!" << std::endl;
        break;
      }
//      int ts = 0;
//      for(int i=0;i<thread_num;++i){
//        while(!pq.empty() && !pq.top().splittable())pq.pop();
//        if(pq.empty())break;
//        std::async(adt::split,pq.top(),gen,[this, &s](double sx, double sy) {
//          return get_pixel(sx / adt::pixel_size, sy / adt::pixel_size, s);
//        },nullptr,nullptr);
//
//        pq.pop();
//        ++ts;
//      }

      auto sq = pq.top();
      pq.pop();
      if (!sq.splittable())continue;
      //std::cerr << "splitting score: " << sq.score << std::endl;
      adt::square sons[4];
      adt::split(sq,gen, [this, &s](double sx, double sy) {
        return get_pixel(sx / adt::pixel_size, sy / adt::pixel_size, s);
      },sons,sons+4);
      r+=3;
      for(auto &s : sons)s.draw(scr,wire);
      //scr.flush();
      for(auto &s : sons)pq.push(s);
      //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return r;
  });
  return {std::move(p), std::move(f)};
}


/*void render(std::string filename, const scene<D>& s)
{
    unsigned char *tmp_mem = new unsigned char [height_px*width_px*3];
    unsigned char *i = tmp_mem;
    #pragma omp parallel for
    for(int y=0;y<height_px;y++)
    {
        unsigned char *i = tmp_mem + y*width_px*3;
    for(int x=0;x<width_px;x++,i+=3)
    {

        *(int *)i = get_pixel(x,y,s).tonemap().to_rgb_le();
        if(x==0 && y%10==0)fprintf(stderr,"%f %%\n",((D)y+1)*100/height_px);
        //*(int *)i = ((x>200)?(color_rgb<D>::from_rgb(0x333333)):(color_rgb<D>(0))).tonemap().to_rgb_le();
    }
    }
    lodepng_encode24_file(filename.c_str(),tmp_mem,width_px,height_px);
    delete[] tmp_mem;
}*/
};
