#ifndef _INCLUDE_CAMERA_H_
#define _INCLUDE_CAMERA_H_

#include <iostream>
#include <vector>
#include "types.h"
#include <math.h>
#include <future>

namespace rtx{
class camera
{
public:
    vector3 position, depth, vertical;
    int width_px,height_px,bounces;
    dtype fov;
    dtype width_m,height_m;
    dtype aspect_ratio;
    dtype x_step_m,y_step_m;
    camera(const vector3& p,const vector3& d,const vector3& v, int w,int h,dtype f = 45.0,int b = 2);
    ray3 cast_ray(int x, int y) const;
    ray3 cast_ray(dtype x, dtype y) const;
  color_rgb get_pixel(int x, int y, const scene& s) const;
  color_rgb get_pixel_d(double x, double y, const scene& s) const;
  void render_screen(screen& scr, const scene& s);
  void render_screen_adaptive(screen& scr, const scene& s);
  std::tuple<std::promise<void>,std::future<size_t>> render_screen_2(screen& scr, const scene& s,bool wire=false,size_t rays=1000);
  std::tuple<std::promise<void>,std::future<size_t>> render_screen_3(screen& scr, const scene& s);
  std::tuple<std::promise<void>,std::future<size_t>> render_screen_4(screen& scr, const scene& s,bool wire=false);
  std::tuple<std::promise<void>,std::future<size_t>> render_screen_5(screen& scr, const scene& s,bool wire=false);
};

};
#endif
