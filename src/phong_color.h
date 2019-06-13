#ifndef _INCLUDE_PHONG_COLOR_H_
#define _INCLUDE_PHONG_COLOR_H_

#include <iostream>
#include "rtx.h"

namespace rtx{
    template<typename T>
    class phong_color : public scene_object {
        static_assert(std::is_base_of<scene_object, T>::value, "T must derive from scene_object");
    private:
        T internal;
        color_rgb color_ambient,color_diffuse,color_specular;
        dtype alpha;
    public:
        
    template <typename... Ts>
    phong_color(const color_rgb& a,const color_rgb& d,const color_rgb& s,dtype alp,Ts&&... args) : internal(std::forward<Ts>(args)...),color_ambient(a),color_diffuse(d),color_specular(s),alpha(alp) {} ;
    
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
        //ambient component
        color_rgb c=color_ambient*s.ambient_color;
        //for each light
        vector3 n = get_normal_at(p);
        vector3 vw = -r.d;
        for(light* l:s.lights()){
            color_rgb il = l->illuminate(s,this,p);
            if(il.magnitude2()<eps)continue;
            vector3 vl = -(l->get_incident_at(p));
            //diffuse component
            c+=il*color_diffuse*std::max<dtype>(0,n.dot(vl));
            //specular component
            vector3 vr = n*(n.dot(vl))*2 - vl;
            vr.normalise();
            c+=il*color_specular*std::pow(std::max<dtype>(0,vw.dot(vr)),alpha);
        }
        return c;
    }
    
    virtual uint64_t instance_id() const override {
        return internal.instance_id();
    }
    
    };
};
#endif
