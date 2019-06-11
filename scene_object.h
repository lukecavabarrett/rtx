#ifndef _INCLUDE_SCENE_OBJECT_H_
#define _INCLUDE_SCENE_OBJECT_H_

#include <iostream>
#include "typedefs.h"
#include "vector3.h"
#include "color_rgb.h"
#include "ray3.h"

namespace rtx{
    class scene_object{
    public:
    virtual bool is_in(const vector3& v) const;
    virtual color_rgb get_color_at(const vector3& v) const;
    virtual vector3 get_normal_at(const vector3& v) const = 0;
    virtual vector3 get_mapping_at(const vector3& v) const;
    virtual std::pair<bool,vector3> intersection_with(const ray3& r) const = 0;
    virtual color_rgb trace(const vector3& p,const ray3& r,const scene& s,int bounces=0) const = 0;
    };
};
#endif
