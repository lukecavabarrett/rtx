#ifndef _INCLUDE_REFLECT_H_
#define _INCLUDE_REFLECT_H_

#include <iostream>
#include "rtx.h"

namespace rtx{
    template<typename T>
    class reflect : public scene_object {
        static_assert(std::is_base_of<scene_object, T>::value, "T must derive from scene_object");
    private:
        T internal;
        dtype rate;
    public:
        
    template <typename... Ts>
    reflect(dtype r,Ts&&... args) : internal(std::forward<Ts>(args)...),rate(std::min<dtype>(std::max<dtype>(0,r),1)) {} ;
    
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
        
        if(bounces){
            vector3 n = get_normal_at(p);
            vector3 vr = r.d - n*(n.dot(r.d))*2;
            vr.normalise();
            ray3 ref(p+vr*eps,vr);
            return internal.trace(p,r,s,bounces)*(1-rate)+s.trace(ref,bounces-1)*rate;
        } else {
            return internal.trace(p,r,s,bounces);
        }
    }
    
    virtual uint64_t instance_id() const override {
        return internal.instance_id();
    }
    
    };
};
#endif
