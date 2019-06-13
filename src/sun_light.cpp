#include "rtx.h"
#include <cstdlib>
#include <cmath>
#include <limits>
#include <tuple>

namespace rtx{
    sun_light::sun_light (const vector3& v,const color_rgb& c) : direction(v.normalised()), color(c) {} 
    vector3 sun_light::get_incident_at(const vector3& v) const {
        return direction;
    }
    color_rgb sun_light::illuminate(const scene& s,const scene_object* o,const vector3& v) const {
        if(std::get<2>(s.cast_ray(ray3(v,-direction)))==nullptr)return color;
        return color_rgb(0);
    }
};
