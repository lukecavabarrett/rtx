#ifndef _INCLUDE_SPHERE_H_
#define _INCLUDE_SPHERE_H_

#include <iostream>
#include "typedefs.h"

namespace rtx{
    class sphere : public scene_object {
    private:
        vector3 position;
        dtype radius;
    public:
    sphere(const vector3& p,dtype r=1);
    
    
    /*vector3 get_normal_at(const vector3& v) const;
    virtual vector3 get_mapping_at(const vector3& v) const;
    virtual std::pair<dtype,vector3> intersection_with(const ray3& r) const;
    virtual color_rgb trace(const vector3& p,const ray3& r,const scene& s,int bounces=0) const;*/
    
    virtual vector3 get_normal_at(const vector3& v) const override;
    virtual vector3 get_mapping_at(const vector3& v) const override;
    virtual std::pair<dtype,vector3> intersection_with(const ray3& r) const override;
    virtual color_rgb trace(const vector3& p,const ray3& r,const scene& s,int bounces=0) const override;
    };
};
#endif
