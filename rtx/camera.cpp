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
ray3 camera::cast_ray(int x, int y) const {
  dtype x_pos = (x_step_m - width_m) / 2 + x * x_step_m;
  dtype y_pos = (y_step_m + height_m) / 2 - y * y_step_m;
  throw;
  return ray3(position, (depth + vertical * y_pos + depth.cross(vertical) * x_pos).normalised());
}
ray3 camera::cast_ray(dtype x, dtype y) const {
  dtype x_pos = (x_step_m - width_m) / 2 + x * x_step_m;
  dtype y_pos = (y_step_m + height_m) / 2 - y * y_step_m;
  ray3 r(position, (depth + vertical * y_pos + depth.cross(vertical) * x_pos).normalised());
  constexpr dtype focal_length = 5, move = focal_length*0.05;
  //constexpr dtype focal_length = 5, move = 0*focal_length;
  auto np = position + vertical*(vector3::random_01()-0.5)*move+ depth.cross(vertical)*(vector3::random_01()-0.5)*move;// vector3::random_inside_unit_sphere()*0.0025;
  return ray3(np, r.evaluate_at(focal_length)-np);
}
color_rgb camera::get_pixel(int x, int y, const scene &s) const {
  return s.trace(cast_ray(x, y), bounces);
}

color_rgb camera::get_pixel_d(double x, double y, const scene &s) const {
  return s.trace(cast_ray(x, y), bounces);
}
void camera::render_screen(screen &scr, const scene &s) {
#pragma omp parallel for
  for (int y = 0; y < height_px; y++) {
    for (int x = 0; x < width_px; x++) {

      scr.put_pixel(x, y, get_pixel(x, y, s).tonemap().to_rgb());
      //*(int *)i = ((x>200)?(color_rgb<D>::from_rgb(0x333333)):(color_rgb<D>(0))).tonemap().to_rgb();
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
        auto c = get_pixel(x, y, s).tonemap().to_rgb();
        rendered_points++;
        for (int yp = y; yp < std::min(y + square_size, height_px); ++yp)
          for (int xp = x; xp < std::min(x + square_size, width_px); ++xp)
            scr.put_pixel(xp, yp, c);
        //*(int *)i = ((x>200)?(color_rgb<D>::from_rgb(0x333333)):(color_rgb<D>(0))).tonemap().to_rgb();
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
constexpr size_t pixel_size = 8;
color_rgb pixel_buf[1920][1080];
struct square {
  double score;
  size_t x, y, size; // these are expressed in pixel_size coordinates
  size_t sx, sy; // the sampled point
  color_rgb color;
  bool operator<(const square &o) const { return score < o.score; }
  void undraw(screen &scr) const {
    if (size > pixel_size)return;
    scr.put_pixel(x / pixel_size, y / pixel_size, 0xff00ff);
    if (size == pixel_size) {
      pixel_buf[x / pixel_size][y / pixel_size] = color_rgb(0);
    } else
      pixel_buf[x / pixel_size][y / pixel_size] -= (color.tonemap() * size * size) / (pixel_size * pixel_size);
  }
  void draw(screen &scr, bool wire = false) const {
    if (size >= pixel_size) {
      //draw
      scr.put_rectangle(x / pixel_size, y / pixel_size, size / pixel_size, size / pixel_size, color.tonemap().to_rgb());
      //scr.put_pixel(x/pixel_size,y/pixel_size,color.tonemap().to_rgb());
      //bounding box
      if (wire)scr.put_rectangle(x / pixel_size, y / pixel_size, size / pixel_size, size / pixel_size, 0xffd300, false);
    } else {
      pixel_buf[x / pixel_size][y / pixel_size] += (color.tonemap() * size * size) / (pixel_size * pixel_size);
    }
  }
  void checkout_draw(screen &scr) const {
    if (size >= pixel_size)return;
    //if(size >= pixel_size/2 )return;
    scr.put_pixel(x / pixel_size, y / pixel_size, pixel_buf[x / pixel_size][y / pixel_size].to_rgb());
  }

  bool splittable() const { return size > 1; }
};
template<typename OutIt>
void split(square source, const camera *cmr, const scene *scr, OutIt begin, OutIt end) {
  //assert(source.size > pixel_size);
  bool plus_y = false, plus_x = false;
  vector3 mean = 0;
  for (auto it = begin; it != end; ++it) {
    square &s = *it;
    s = source;
    s.size >>= 1;
    if (plus_x)s.x += s.size;
    if (plus_y)s.y += s.size;
    if (s.sx < s.x || s.sx >= s.x + s.size || s.sy < s.y || s.sy >= s.y + s.size) {
      //regenerate
      s.sx = s.x + (rand() & (s.size - 1));
      s.sy = s.y + (rand() & (s.size - 1));
      s.color = /*s.y>10*pixel_size ? color_rgb(0) : (std::sin(std::pow(double(s.sx)/(pixel_size*10),2))>0);*/cmr->get_pixel_d(s.sx / adt::pixel_size, s.sy / adt::pixel_size, *scr);
    }
    mean += s.color;
    s.score = 0;
    if (plus_x)plus_y ^= 1;
    plus_x ^= 1;
  }
  mean /= 4;
  double variance = 0;
  for (auto it = begin; it != end; ++it)variance += mean.distance(it->color);
  variance /= 4;
  //std::cerr << "variance is " << variance << std::endl;
  constexpr double eps = 0.000001;
  for (auto it = begin; it != end; ++it)it->score = (variance + eps) * it->size * it->size;
}
}

template<typename R>
bool is_ready(std::future<R> const &f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

std::tuple<std::promise<void>, std::future<size_t>> camera::render_screen_2(screen &scr, const scene &s, bool wire, size_t rays_limit) {
  //scr.put_rectangle(0,0,width_px, height_px,0xff00ff);
  //static std::random_device rd;  //Will be used to obtain a seed for the random number engine
  static std::mt19937 gen(1234); //Standard mersenne_twister_engine seeded with rd()
  //static std::uniform_real_distribution<> dis(0.0, 1.0);
  //for(auto& cs : adt::pixel_buf)for(auto& c : cs)c=color_rgb(0);
  std::promise<void> p;
  std::future<size_t> f = std::async([this, &scr, &s, wire, fut = p.get_future()]() -> size_t {
    size_t square_size = 1;
    size_t r = 0;
    while ((height_px % (square_size << 1) == 0) && (width_px % (square_size << 1) == 0))square_size <<= 1;
    std::priority_queue<adt::square> pq;
    for (size_t y = 0; y < height_px; y += square_size)
      for (size_t x = 0; x < width_px; x += square_size) {
        ++r;
        size_t sx = x * adt::pixel_size + std::uniform_int_distribution<>(0, square_size * adt::pixel_size - 1)(gen);
        size_t sy = y * adt::pixel_size + std::uniform_int_distribution<>(0, square_size * adt::pixel_size - 1)(gen);
        color_rgb c = get_pixel_d(double(sx) / adt::pixel_size, double(sy) / adt::pixel_size, s);
        adt::square s = {.score = 1000000000, .x=x * adt::pixel_size, .y=y * adt::pixel_size, .size=square_size * adt::pixel_size, .sx=sx, .sy=sy, .color=c};
        s.draw(scr, wire);
        pq.push(s);
      }
    constexpr int thread_num = 16;
    adt::square outs[thread_num * 4];
    std::future<void> trs[thread_num];
    size_t smallest = -1;
    while (true) {
      if (is_ready(fut)) {
        std::cerr << "early exit!" << std::endl;
        break;
      }
//#define MULTITHREAD
#ifdef MULTITHREAD

      int ts = 0;
      for(int i=0;i<thread_num;++i){
        while(!pq.empty() && !pq.top().splittable())pq.pop();
        if(pq.empty())break;
        trs[i] = std::async(adt::split<adt::square*>,pq.top(),this,&s,outs+i*4,outs+i*4+4);
        pq.top().undraw();
        pq.pop();
        ++ts;
        r+=3;
      }
      if(ts==0)return r;
      for(int i=0;i<ts;++i)trs[i].wait();
      for(int i=0;i<ts*4;++i){
        outs[i].draw(scr,wire);
        outs[i].checkout_draw(scr);
        pq.push(outs[i]);
        if(outs[i].size<smallest){
          smallest = outs[i].size;
          std::cerr << "smallest: "<<smallest<<std::endl;
        }
      }
#else
      while (!pq.empty() && !pq.top().splittable())pq.pop();
      if (pq.empty())break;
      adt::split<adt::square *>(pq.top(), this, &s, outs, outs + 4);
      pq.top().undraw(scr);
      pq.pop();
      r += 3;
      for (int i = 0; i < 4; ++i) {
        outs[i].draw(scr, wire);
        outs[i].checkout_draw(scr);
        pq.push(outs[i]);
        if (outs[i].size < smallest) {
          smallest = outs[i].size;
          std::cerr << "smallest: " << smallest << std::endl;
        }
      }
#endif
#undef MULTITHREAD

      //scr.flush();
      //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cerr << "finished render!" << std::endl;
    return r;
  });
  return {std::move(p), std::move(f)};
}

std::tuple<std::promise<void>, std::future<size_t>> camera::render_screen_3(screen &scr, const scene &s) {
  std::promise<void> p;

  auto f = std::async([this, & scr, &s, fut = p.get_future()]() -> size_t {
    static std::mt19937 gen(1234); //Standard mersenne_twister_engine seeded with rd()
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    static color_rgb screen_buffer[1920][1080]; // the screen buffer, already tonemapped. 0, or the currently drawn color
    size_t r = 0;
    constexpr size_t pixel_size = 8;
    size_t l = 0;
    for(size_t px = 0;px<pixel_size;++px)
      for(size_t py = 0;py<pixel_size;++py){
        ++l;
      for (size_t x = 0; x < width_px; ++x)
      for (size_t y = 0; y < width_px; ++y) {
        if (is_ready(fut))return r;
        if(l==1)screen_buffer[x][y]=color_rgb(0);
        screen_buffer[x][y] += get_pixel_d(double(x*pixel_size + px + dis(gen)) / pixel_size, double(y*pixel_size + py + dis(gen)) / pixel_size, s).tonemap();
        r ++;
        scr.put_pixel(x, y, color_rgb(screen_buffer[x][y]/l).to_rgb());
      }}
    return r;
  });
  return {std::move(p), std::move(f)};

}

namespace adt4 {

constexpr size_t pixel_size = 8; // the smallest jittering unit. each pixel is a square of pixel_size^2 jittering unit
constexpr size_t square_sample_size = 8; // each square has square_sample_size^2 samples.

struct crd { size_t x, y; };
struct crd_c { size_t x, y;color_rgb c; };
inline double make_score(size_t size, double var) {
  constexpr double eps = 0.05;
  return var + std::log2(double(size))/6;
  return (var+eps)*size;
  //consider 4 cases:
  // A variance nearly zero, size quite big (i.e. 32x32 pixels)
  // B variance nearly zero, size around few pixels (i.e. 4x4)
  // C variance nearly 0.6, size quite big (i.e. 16x16 pixels)
  // D variance nearly 0.6, size around few pixels (i.e. 4x4)
  // E variance nearly 0.6, size around sub_pixels (i.e. 0.25x0.25)
  // We'd like  C > A > D > E > B
  return (var*var + eps) * size * size * size;
}
namespace test {
//static constexpr double scA = make_score(pixel_size * 32, 0.0);
//static constexpr double scB = make_score(pixel_size * 4, 0.0);
//static constexpr double scC = make_score(pixel_size * 16, 0.6);
//static constexpr double scD = make_score(pixel_size * 4, 0.6);
//static constexpr double scE = make_score(pixel_size / 4, 0.6);
//static_assert(scC > scA);
//static_assert(scA > scD); // if this fail, need to give more score to big (low variance) squares
//static_assert(scD > scE);
//static_assert(scE > scB); // if this fails, need to give more power to high variance (small) squares

}
struct square {
  static inline color_rgb screen_buffer[1920][1080]; // the screen buffer, already tonemapped. 0, or the currently drawn color
  static inline std::priority_queue<square> pq;

  crd orig;//upper left corner
  size_t size; //size between samples
  std::array<std::array<crd_c, square_sample_size>, square_sample_size> jitter_pos;
  double score;
  bool splittable() const {
    return size > 1;
  }
  bool operator<(const square &o) const { return score < o.score; }
  square(const square &) = delete;
  square(square &&) = default;
  square &operator=(const square &) = delete;
  square &operator=(square &&) = default;
  inline static double randd() { return double(rand() & 127) / 128; }
  square(size_t s, crd o, const camera &cam, const scene &sce, screen &scr, bool wire = false) : size(s), orig(o), score(size) { //initial_constructor
    assert(s >= pixel_size);
    //1. generate jitter_pos, sample and render
    color_rgb mean = 0;
    for (size_t x = 0; x < square_sample_size; ++x) {
      for (size_t y = 0; y < square_sample_size; ++y) {
        jitter_pos[x][y] = crd_c{.x = orig.x + size * x + rand() % size, .y = orig.y + size * y + rand() % size};
        color_rgb c = cam.get_pixel_d(double(jitter_pos[x][y].x + randd()) / pixel_size, double(jitter_pos[x][y].y + randd()) / pixel_size, sce).tonemap();//TODO:: add noise [0,1)
        //screen_buffer[jitter_pos[x][y].x][jitter_pos[x][y].y] = c;
        jitter_pos[x][y].c = c;
        mean += c;
        scr.put_rectangle(size_t(orig.x + size * x) / pixel_size, size_t(orig.y + size * y) / pixel_size, size / pixel_size, size / square_sample_size, c.to_rgb());
        if (wire)scr.put_rectangle(size_t(orig.x + size * x) / pixel_size, size_t(orig.y + size * y) / pixel_size, size / pixel_size, size / square_sample_size, 0xffd300, false);
        if (wire)scr.put_pixel(jitter_pos[x][y].x / pixel_size, jitter_pos[x][y].y / pixel_size, 0xff00ff);
      }
    }
    mean /= square_sample_size * square_sample_size;
    double var = 0;
    for (auto &jps : jitter_pos)for (auto &jp : jps)var += mean.distance(jp.c);
    var /= square_sample_size * square_sample_size;
    //var = std::sqrt(var);
    score = make_score(size, var);
  }
  static constexpr size_t pixel_area = pixel_size * pixel_size;
  static constexpr double inv_pixel_area = double(1) / pixel_area;
  static inline bool in_range(size_t x, size_t bx, size_t size) { return x >= bx && x < bx + size; }
  square(const square &s, bool dx, bool dy, const camera &cam, const scene &sce, screen &scr, bool wire = false) : size(s.size / 2), orig(s.orig), score(size) {
    if (dx)orig.x += size * square_sample_size;
    if (dy)orig.y += size * square_sample_size;
    color_rgb mean = 0;
    for (size_t x = 0; x < square_sample_size; ++x) {
      for (size_t y = 0; y < square_sample_size; ++y) {
        //TODO: reuse sampled
        jitter_pos[x][y] = s.jitter_pos[x / 2 + (square_sample_size / 2) * dx][y / 2 + (square_sample_size / 2) * dy];
        const bool sampled = in_range(jitter_pos[x][y].x, orig.x + size * x, size) && in_range(jitter_pos[x][y].y, orig.y + size * y, size);
        if (!sampled) {
          jitter_pos[x][y] = crd_c{.x = orig.x + size * x + rand() % size, .y = orig.y + size * y + rand() % size};
          color_rgb c = cam.get_pixel_d(double(jitter_pos[x][y].x + randd()) / pixel_size, double(jitter_pos[x][y].y + randd()) / pixel_size, sce).tonemap();
          jitter_pos[x][y].c = c;
        }
        color_rgb c = jitter_pos[x][y].c;
        mean += c;
//#define AVERAGED_FILL
#ifndef AVERAGED_FILL
        if (size >= pixel_size) {
          scr.put_rectangle((orig.x + size * x) / pixel_size, (orig.y + size * y) / pixel_size, size / pixel_size, size / pixel_size, c.to_rgb());
          if (wire)scr.put_rectangle((orig.x + size * x) / pixel_size, (orig.y + size * y) / pixel_size, size / pixel_size, size / pixel_size, 0xffd300, false);
          if (wire)scr.put_pixel(jitter_pos[x][y].x / pixel_size, jitter_pos[x][y].y / pixel_size, 0x00ff00);
        } else {
          screen_buffer[jitter_pos[x][y].x / pixel_size][jitter_pos[x][y].y / pixel_size] += c * double(size * size) * inv_pixel_area;
          //TODO: optimize put_pixel
          scr.put_pixel(jitter_pos[x][y].x / pixel_size, jitter_pos[x][y].y / pixel_size, screen_buffer[jitter_pos[x][y].x / pixel_size][jitter_pos[x][y].y / pixel_size].to_rgb());
        }
#endif
      }
    }
    mean /= square_sample_size * square_sample_size;
#ifdef AVERAGED_FILL
    for (size_t x = 0; x < square_sample_size; ++x) {
      for (size_t y = 0; y < square_sample_size; ++y) {
        if (size >= pixel_size) {
          scr.put_rectangle((orig.x + size * x) / pixel_size, (orig.y + size * y) / pixel_size, size / pixel_size, size / pixel_size, mean.to_rgb());
          if (wire)scr.put_rectangle((orig.x + size * x) / pixel_size, (orig.y + size * y) / pixel_size, size / pixel_size, size / pixel_size, 0xffd300, false);
          if (wire)scr.put_pixel(jitter_pos[x][y].x / pixel_size, jitter_pos[x][y].y / pixel_size, 0x00ff00);
        }else {
          screen_buffer[jitter_pos[x][y].x / pixel_size][jitter_pos[x][y].y / pixel_size] += mean * double(size * size) * inv_pixel_area;
          //TODO: optimize put_pixel
          scr.put_pixel(jitter_pos[x][y].x / pixel_size, jitter_pos[x][y].y / pixel_size, screen_buffer[jitter_pos[x][y].x / pixel_size][jitter_pos[x][y].y / pixel_size].to_rgb());
        }
      }
    }
#undef AVERAGED_FILL
#endif
    double var = 0;
    for (auto &jps : jitter_pos)for (auto &jp : jps)var += mean.distance(jp.c);
    var /= square_sample_size * square_sample_size;
    score = make_score(size, var);
  }
  void remove_bf() {
    if (size > pixel_size)return;
    if (size == pixel_size) {
      //zeros
      for (size_t x = 0; x < square_sample_size; ++x)
        for (size_t y = 0; y < square_sample_size; ++y)
          screen_buffer[orig.x / pixel_size + x][orig.y / pixel_size + y] = color_rgb(0);
    } else {
      for (size_t x = 0; x < square_sample_size; ++x)
        for (size_t y = 0; y < square_sample_size; ++y)
          screen_buffer[jitter_pos[x][y].x / pixel_size][jitter_pos[x][y].y / pixel_size] -= jitter_pos[x][y].c * size * size * inv_pixel_area;
    }
  }
  static bool extract_repush(const camera &cam, const scene &sce, screen &scr, bool wire = false) {
    while (!pq.empty() && !pq.top().splittable())pq.pop();
    if (pq.empty())return false;
    using namespace std::chrono_literals;
    square sq = std::move(const_cast<square &>(pq.top()));
    pq.pop();
    sq.remove_bf();
    pq.emplace(sq, false, false, cam, sce, scr, wire);
    pq.emplace(sq, false, true, cam, sce, scr, wire);
    pq.emplace(sq, true, false, cam, sce, scr, wire);
    pq.emplace(sq, true, true, cam, sce, scr, wire);
    return true;
  }
};
}

std::tuple<std::promise<void>, std::future<size_t>> camera::render_screen_4(screen &scr, const scene &s, bool wire) {
  using namespace adt4;

  //in_square_size/=square_sample_size;

  std::promise<void> p;
  std::future<size_t> f = std::async([cam = *this, &sce = s, &scr, fstop = p.get_future(), wire,width_px=width_px,height_px=height_px]() -> size_t {
  size_t in_square_size = 1;
  size_t rays = 0;
  while ((height_px % (in_square_size << 1) == 0) && (width_px % (in_square_size << 1) == 0))in_square_size <<= 1;
  in_square_size *= pixel_size;
  square::pq = std::priority_queue<square>();
  for (size_t x = 0; x < width_px * pixel_size; x += in_square_size)
    for (size_t y = 0; y < height_px * pixel_size; y += in_square_size) {
      square::pq.emplace<size_t, crd, const camera &, const scene &, screen &>(in_square_size / square_sample_size, {.x=x, .y=y}, cam, sce, scr, wire);
      rays+=square_sample_size*square_sample_size;
    }

    size_t it = 0;
    while (square::extract_repush(cam, sce, scr, wire)) {
      rays+=square_sample_size*square_sample_size*3;
      if ((it & 1) == 0) {
        scr.flush();
        if (is_ready(fstop))break;
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      ++it;
    }
    std::cerr << "frame done" << std::endl;
    return rays;
  });
  return {std::move(p), std::move(f)};
}

namespace adt4{

struct var_accumulator{
  vector3 sum=0,sum_sq=0;
  size_t n_samples=0;
  void add(const vector3& v){
    sum+=v;
    sum_sq+=v.squared();
    ++n_samples;
  }
  void rm(const vector3& v){
    sum-=v;
    sum_sq-=v.squared();
    --n_samples;
  }
  vector3 var() const {
    return (sum_sq/n_samples) - (sum/n_samples).squared();
  }
};

namespace super_sampling{

constexpr auto n_sumpersamples (int d)  {
  if(d==0)return 1;
  return (n_sumpersamples(d-1)<<2)+1;
}

//struct my_it

template<int d>
struct super_sampling{
  typedef super_sampling<d-1> inner;
  static constexpr size_t n_samples = (inner::n_samples<<2)+1;
  static constexpr std::array<std::tuple<double,double>,n_samples> make_fn(){
    std::array<std::tuple<double,double>,n_samples> v;
    std::get<0>(v.at(0))=0.5;
    std::get<1>(v.at(0))=0.5;
    int i=0;
    for(auto [x,y] : inner::xy_samples){
      for(int py=0;py!=2;++py)for(int px=0;px!=2;++px){
          ++i;
          std::get<0>(v.at(i))=x/2 + (px ? 0.5 : 0);
          std::get<1>(v.at(i))=y/2 + (py ? 0.5 : 0);
        }
    }
    return v;
  }
  static constexpr std::array<std::tuple<double,double>,n_samples> xy_samples = make_fn();
};
template<>
struct super_sampling<0>{
  static constexpr size_t n_samples = 1;
  static constexpr std::array<std::tuple<double,double>,1> xy_samples = {std::tuple<double,double>{0.5,0.5}};
};

static_assert(super_sampling<2>::n_samples==21);
static_assert(super_sampling<2>::xy_samples[4]==std::tuple<double,double>({0.75,0.75}));

}


}

std::tuple<std::promise<void>, std::future<size_t>> camera::render_screen_5(screen &scr, const scene &s, bool wire) {
  return std::tuple<std::promise<void>, std::future<size_t>>();
}

};
