#ifndef _INCLUDE_LIGHT_H_
#define _INCLUDE_LIGHT_H_

#include "types.h"

namespace rtx{
    class light {
    public:
    virtual vector3 get_incident_at(const vector3& v) const = 0;
    virtual color_rgb illuminate(const scene& s,const scene_object* o,const vector3& v) const = 0;
    };
};
#endif
