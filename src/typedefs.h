#ifndef _INCLUDE_TYPEDEFS_H_
#define _INCLUDE_TYPEDEFS_H_

#include <bits/stdint-uintn.h>

namespace rtx{
    
typedef double dtype;
typedef uint32_t utype;
typedef int stype;

constexpr dtype eps = 10e-12;

class vector3;
class vector4;
class color_rgb;
class ray3;
class scene_object;
class scene;
class screen;
class camera;
class sphere;
template<typename T> class solid_color;
class light;
class sun_light;
template<typename T> class phong_color;
};


#endif
