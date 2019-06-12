#ifndef _INCLUDE_SOLID_COLOR_H_
#define _INCLUDE_SOLID_COLOR_H_

#include <iostream>
#include "rtx.h"

namespace rtx{
    template<typename T>
    class solid_color : public scene_object {
        static_assert(std::is_base_of<scene_object, T>::value, "T must derive from scene_object");
    private:
        T internal;
        color_rgb color;
    public:
        
    template <typename... Ts>
    solid_color(const color_rgb& c,Ts&&... args) : internal(std::forward<Ts>(args)...),color(c) {} ;
    
    virtual vector3 get_normal_at(const vector3& v) const override {
        return internal.get_normal_at(v);
    }
    
    virtual vector3 get_mapping_at(const vector3& v) const override {
        return internal.get_mapping_at(v);
    }
    
    virtual std::pair<dtype,vector3> intersection_with(const ray3& r) const override {
        return internal.intersection_with(r);
    }
    
    virtual color_rgb trace(const vector3& p,const ray3& r,const scene& s,int bounces=0) const override {
        return color;
    }
    
    };
};
#endif
