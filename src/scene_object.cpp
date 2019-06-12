#include "scene_object.h"
#include <cstdlib>
#include <cmath>
namespace rtx{
    bool scene_object::is_in(const vector3& v) const {return false;}
    color_rgb scene_object::get_color_at(const vector3& v) const {return color_rgb(0);}
    vector3 scene_object::get_mapping_at(const vector3& v) const {return vector3(0);}
};
