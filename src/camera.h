#ifndef _INCLUDE_CAMERA_H_
#define _INCLUDE_CAMERA_H_

#include <iostream>
#include <vector>
#include "typedefs.h"
#include <math.h>

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
    ray3 cast_ray(int x, int y);
    ray3 cast_ray(dtype x, dtype y);
    color_rgb get_pixel(int x, int y, const scene& s);
    void render_screen(screen& scr, const scene& s);
};

};
#endif
