#ifndef COLOR_RGB_H
#define	COLOR_RGB_H

#include "typedefs.h"
#include <iostream>
namespace rtx{
class color_rgb : public vector3,texture
{
public:
    using vector3::vector3;
    using vector3::operator=;
    static constexpr dtype inv_gamma = 1.0/2.2;
    static constexpr dtype g_a = 2.0;
    static constexpr dtype g_b = 1.3;
private:
    static int to_byte(dtype v);
public:
    color_rgb (const vector3& v);
    //color_rgb (dtype r,dtype g,dtype b);
    int to_rgb() const;
    static color_rgb from_rgb(int w);
    color_rgb get_color(const vector3& v) const;
    int to_rgb_le() const;
    const vector3& to_vec() const;
    color_rgb tonemap() const;
    friend std::ostream& operator<<(std::ostream& os, const color_rgb& v);
};
};
#endif
