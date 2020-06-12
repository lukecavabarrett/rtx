#ifndef _INCLUDE_RAY3_H_
#define _INCLUDE_RAY3_H_

#include <iostream>
#include "types.h"
namespace rtx{
class ray3
{
public:
    vector3 o,d;
    ray3(const vector3& oo, const vector3& dd);
    const vector3& get_origin() const;
    const vector3& get_direction() const;
    vector3 evaluate_at(dtype s) const;
};
};
#endif
