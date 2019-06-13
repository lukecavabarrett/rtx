#ifndef _INCLUDE_SCENE_H_
#define _INCLUDE_SCENE_H_

#include <iostream>
#include <vector>
#include "typedefs.h"

namespace rtx{
    class scene{
        std::vector<scene_object*> obj_vp;
        std::vector<light*> lgt_vp;
    public:
        color_rgb ambient_color,background_color;
        scene(const color_rgb& ambient,const color_rgb& background);
        const std::vector<scene_object*>& objects() const;
        const std::vector<light*>& lights() const;
        void add(scene_object* p);
        //TODO add light
        std::tuple<dtype,vector3,scene_object*> cast_ray(const ray3& r) const;
        color_rgb trace(const ray3& r,int bounces=0) const;
        
    };
};
#endif
