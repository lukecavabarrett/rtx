#include "rtx.h"
#include <cstdlib>
#include <cmath>
namespace rtx{
ray3::ray3(const vector3& oo, const vector3& dd)
    {
        o = oo;
        d = dd;
        d.normalise();
    }
    
    const vector3& ray3::get_origin() const {return o;}
    const vector3& ray3::get_direction() const {return d;}
    vector3 ray3::evaluate_at(dtype s) const {return o+d*s;}
};
