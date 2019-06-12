#ifndef _INCLUDE_SCENE_OBJECT_H_
#define _INCLUDE_SCENE_OBJECT_H_

#include <iostream>
#include "typedefs.h"

namespace rtx{
    class scene_object{
    public:
    virtual bool is_in(const vector3& v) const;
    virtual color_rgb get_color_at(const vector3& v) const;
    virtual vector3 get_normal_at(const vector3& v) const;
    virtual vector3 get_mapping_at(const vector3& v) const;
    virtual std::pair<dtype,vector3> intersection_with(const ray3& r) const;
    virtual color_rgb trace(const vector3& p,const ray3& r,const scene& s,int bounces=0) const;
    };
};
#endif
