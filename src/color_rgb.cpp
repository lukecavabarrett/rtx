#include "rtx.h"
#include <cstdlib>
#include <cmath>
#include <iomanip>
namespace rtx{
color_rgb::color_rgb (const vector3& v)
{
    vector3::x=v.x;
    vector3::y=v.y;
    vector3::z=v.z;
}

int color_rgb::to_byte(dtype v)
{
    return (int)(255 * std::max<dtype>(0, std::min<dtype>(1,v)));
}

int color_rgb::to_rgb() const
{
    return to_byte(vector3::x)<<16 | to_byte(vector3::y)<<8 | to_byte(vector3::z);
}

color_rgb color_rgb::from_rgb(int w)
{
    return color_rgb(((dtype)((w>>16)&0xff))/255.0,((dtype)((w>>8)&0xff))/255.0,((dtype)(w&0xff))/255.0);
}

int color_rgb::to_rgb_le() const
{
    return to_byte(vector3::z)<<16 | to_byte(vector3::y)<<8 | to_byte(vector3::x);
}

const vector3& color_rgb::to_vec() const
{
    return (const vector3&)(*this);
}

color_rgb color_rgb::tonemap() const
{
    vector3 powRGB = to_vec().relu().power(g_b);
    return color_rgb((powRGB*((powRGB+std::pow(0.5/g_a,g_b)).inverse())).power(inv_gamma));
}

std::ostream& operator<<(std::ostream& os, const color_rgb& v)
{
    os << '#' << std::setfill('0') << std::setw(6) << std::uppercase << std::hex << v.to_rgb();
    return os;
}
};
