#include "rtx.h"
#include <cstdlib>
#include <cmath>
#include <limits>

namespace rtx{
    bool scene_object::is_in(const vector3& v) const {return false;}
    color_rgb scene_object::get_color_at(const vector3& v) const {return color_rgb(0);}
    vector3 scene_object::get_mapping_at(const vector3& v) const {return vector3(0);}
    vector3 scene_object::get_normal_at(const vector3& v) const {return vector3(0);}
    std::pair<dtype,vector3> scene_object::intersection_with(const ray3& r) const {
        return std::pair<dtype,vector3>(55,vector3(0));
        return std::pair<dtype,vector3>(std::numeric_limits<dtype>::infinity(),vector3(0));
    }
    color_rgb scene_object::trace(const vector3& p, const ray3& r, const scene& s, int bounces) const {
        return color_rgb(0);
    };
};
