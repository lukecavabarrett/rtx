#include "rtx.h"
#include <cstdlib>
#include <cmath>
#include <limits>

namespace rtx{
    sphere::sphere(const vector3& p,dtype r) : position(p), radius(r) {}
    vector3 sphere::get_normal_at(const vector3& v) const {
        return (v-position).normalised();
    }
    vector3 sphere::get_mapping_at(const vector3& v) const {
        vector3 pu(1,0,0);
        vector3 n = get_normal_at(v);
        vector3 pv = n.cross(pu);
        return vector3((std::atan2(n.z,n.x)+M_PIl)/(M_PIl*2.0),(1.0-n.y)/2,v.z);
    }
    std::pair<dtype,vector3> sphere::intersection_with(const ray3& r) const {
        dtype a = r.d.magnitude2();
        dtype b = 2 * r.d.dot(r.o-position);
        dtype c = (r.o-position).dot(r.o-position) - radius*radius;
        dtype delta = b*b-4*a*c;
        if(delta<0)return std::pair<dtype,vector3>(std::numeric_limits<dtype>::infinity(),vector3(0));
        delta = std::sqrt(delta);
        dtype s = -b;
        if(s+delta<0)return std::pair<dtype,vector3>(std::numeric_limits<dtype>::infinity(),vector3(0));
        if(s-delta<0)s+=delta; else s-=delta;
        s/=(2*a);
        vector3 p = r.evaluate_at(s);
        return std::pair<dtype,vector3>(s, p);
    }
    color_rgb sphere::trace(const vector3& p,const ray3& r,const scene& s,int bounces) const {return color_rgb(0);}
};
