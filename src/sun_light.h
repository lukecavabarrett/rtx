#ifndef _INCLUDE_SUN_LIGHT_H_
#define _INCLUDE_SUN_LIGHT_H_

#include "typedefs.h"

namespace rtx{
    class sun_light : public light {
    private:
    vector3 direction;
    color_rgb color;
    public:
    sun_light (const vector3& v,const color_rgb& c);
    virtual vector3 get_incident_at(const vector3& v) const;
    virtual color_rgb illuminate(const scene& s,const scene_object* o,const vector3& v) const;
    };
};
#endif
