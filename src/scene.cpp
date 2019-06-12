#include "rtx.h"
#include <cstdlib>
#include <cmath>
#include <tuple>
#include <limits>
namespace rtx{
    scene::scene(const color_rgb& ambient,const color_rgb& background) : ambient_color(ambient), background_color(background) {}
    const std::vector<scene_object*>& scene::objects() const {return obj_vp;}
    void scene::add(scene_object *p){obj_vp.push_back(p);}
    std::tuple<dtype,vector3,scene_object*> scene::cast_ray(const ray3& r) const {
        dtype best_dst=std::numeric_limits<dtype>::infinity();
        vector3 best_v3(0);
        scene_object* best_obj=nullptr;
        for(scene_object *p:obj_vp) {
            std::pair<dtype,vector3> dv=p->intersection_with(r);
            if(dv.first<best_dst){
                best_dst=dv.first;
                best_v3=dv.second;
                best_obj=p;
            }
        }
        return std::tuple<dtype,vector3,scene_object*>(best_dst,best_v3,best_obj);
    }
    color_rgb scene::trace(const ray3& r,int bounces) const {
        std::tuple<dtype,vector3,scene_object*> dvo = cast_ray(r);
        if(std::get<2>(dvo)==nullptr)return background_color;
        return std::get<2>(dvo)->trace(std::get<1>(dvo),r,*this,bounces);
    }
    
};
