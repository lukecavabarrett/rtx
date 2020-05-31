#ifndef _INCLUDE_PLANE_H_
#define _INCLUDE_PLANE_H_

#include <iostream>
#include "typedefs.h"

namespace rtx{
    class plane : public scene_object {
    private:
        vector3 position, normal;
    public:
        plane(const vector3& p,const vector3& n);
        virtual vector3 get_normal_at(const vector3& v) const override;
        virtual vector3 get_mapping_at(const vector3& v) const override;
        virtual std::pair<dtype,vector3> intersection_with(const ray3& r) const override;
        virtual color_rgb trace(const vector3& p,const ray3& r,const scene& s,int bounces=0) const override;
    };
};
#endif
