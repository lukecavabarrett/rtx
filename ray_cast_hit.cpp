#include "ray_cast_hit.h"
#include <cstdlib>
#include <cmath>
namespace rtx{
ray_cast_hit::ray_cast_hit()
    {
        distance = std::numeric_limits<dtype>::infinity();
        hit = nullptr;
    }
    ray_cast_hit::ray_cast_hit(dtype d, const scene_object* h, const vector3& l)
    {
        distance = d;
        hit = h;
        location = l;
    }
    bool ray_cast_hit::is_null()
    {
        return hit==nullptr;
    }
};
