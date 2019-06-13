#ifndef _INCLUDE_LIGHT_H_
#define _INCLUDE_LIGHT_H_

#include "typedefs.h"

namespace rtx{
    class light {
    public:
    virtual vector3 get_incident_at(const vector3& v) const = 0;
    virtual color_rgb illuminate(const vector3& v) const = 0;
    };
};
#endif
