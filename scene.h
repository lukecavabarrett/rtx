#ifndef _INCLUDE_SCENE_H_
#define _INCLUDE_SCENE_H_

#include <iostream>
#include <vector>
#include "typedefs.h"
#include "vector3.h"
#include "color_rgb.h"
#include "ray3.h"
#include "scene_object.h"

namespace rtx{
    class scene{
        color_rgb ambient_color,background_color;
        std::vector<scene_object*> obj_vp;
        //lights
    public:
        scene(const color_rgb& ambient,const color_rgb& background);
        const std::vector<scene_object*>& objects() const;
        void add(scene_object* p);
        std::tuple<dtype,vector3,scene_object*> cast_ray(const ray3& r) const;
        color_rgb trace(const ray3& r,int bounces=0) const;
        
    };
};
#endif
