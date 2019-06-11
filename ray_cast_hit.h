#ifndef _INCLUDE_RAY_CAST_HIT_H_
#define _INCLUDE_RAY_CAST_HIT_H_

#include <iostream>
#include "typedefs.h"
#include "vector3.h"
#include "ray3.h"
namespace rtx{
class ray_cast_hit
{
public:
    dtype distance;
    const scene_object* hit;
    vector3 location;
    ray_cast_hit();
    ray_cast_hit(dtype d, const scene_object* h, const vector3& l);
    bool is_null();
};
};
#endif
