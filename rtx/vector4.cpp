#include "rtx.h"
#include <cstdlib>
#include <cmath>
namespace rtx{
vector4::vector4() : x(0), y(0), z(0), w(0) {}
vector4::vector4(dtype v) : x(v), y(v), z(v), w(v) {}
vector4::vector4(dtype xp,dtype yp,dtype zp,dtype wp) : x(xp), y(yp), z(zp), w(wp) {}
vector4::vector4(const vector4& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
vector4::vector4(const vector3& o,dtype wp) : x(o.x), y(o.y), z(o.z), w(wp) {}

dtype vector4::magnitude() const {return std::sqrt(x*x+y*y+z*z+w*w);}
vector4 vector4::operator+(const vector4& o) const {return vector4(x+o.x,y+o.y,z+o.z,w+o.w);}
void vector4::operator+=(const vector4& o){x+=o.x;y+=o.y;z+=o.z;w+=o.w;}
vector4 vector4::operator-(const vector4& o) const {return vector4(x-o.x,y-o.y,z-o.z,w-o.w);}
void vector4::operator-=(const vector4& o){x-=o.x;y-=o.y;z-=o.z;w-=o.w;}
vector4 vector4::operator*(const vector4& o) const {return vector4(x*o.x,y*o.y,z*o.z,w*o.w);}
void vector4::operator*=(const vector4& o){x*=o.x;y*=o.y;z*=o.z;w*=o.w;}
vector4 vector4::operator/(const vector4& o) const {return vector4(x/o.x,y/o.y,z/o.z,w/o.w);}
void vector4::operator/=(const vector4& o){x/=o.x;y/=o.y;z/=o.z;w/=o.w;}
vector4 vector4::operator+(dtype o) const {return vector4(x+o,y+o,z+o,w+o);}
void vector4::operator+=(dtype o){x+=o;y+=o;z+=o;w+=o;}
vector4 vector4::operator-(dtype o) const {return vector4(x-o,y-o,z-o,w-o);}
vector4 vector4::operator-() const {return vector4(-x,-y,-z,-w);}
void vector4::operator-=(dtype o){x-=o;y-=o;z-=o;w-=o;}
vector4 vector4::operator*(dtype o) const {return vector4(x*o,y*o,z*o,w*o);}
void vector4::operator*=(dtype o){x*=o;y*=o;z*=o;w*=o;}
vector4 vector4::operator/(dtype o) const {return vector4(x/o,y/o,z/o,w/o);}
void vector4::operator/=(dtype o){x/=o;y/=o;z/=o;w/=o;}

vector3 vector4::xyz() const {return vector3(x,y,z);}
color_rgb vector4::rgb() const {return color_rgb(x,y,z);}
int vector4::int_tonemap() {return color_rgb(x,y,z).tonemap().to_rgb();}
dtype vector4::distance(const vector4& o) const {return std::sqrt((x-o.x)*(x-o.x)+(y-o.y)*(y-o.y)+(z-o.z)*(z-o.z)+(w-o.w)*(w-o.w));}
dtype vector4::distance_sqr(const vector4& o) const {return (x-o.x)*(x-o.x)+(y-o.y)*(y-o.y)+(z-o.z)*(z-o.z)+(w-o.w)*(w-o.w);}
dtype vector4::dot(const vector4& o) const {return x*o.x+y*o.y+z*o.z+w*o.w;}
vector4 vector4::power(dtype o) const {return vector4(std::pow(x,o),std::pow(y,o),std::pow(z,o),std::pow(w,o));}
vector4 vector4::inverse() const {return vector4(1/x,1/y,1/z,1/w);}
vector4 vector4::normalised() const {dtype m=magnitude();return vector4(x/m,y/m,z/m,w/m);}
void vector4::normalise(){dtype m=magnitude();x/=m;y/=m;z/=m;w/=m;}
void vector4::swap(vector4& o)
{
    dtype t;
    t=x;x=o.x;o.x=t;
    t=y;y=o.y;o.y=t;
    t=z;z=o.z;o.z=t;
    t=w;w=o.w;o.w=t;
};
vector4 vector4::reflected_in(const vector4& n) const
{
    dtype d = dot(n)*2;
    return vector4(d*n.x-x,d*n.y-y,d*n.z-z,d*n.w-w);
}

dtype vector4::random_01()
{
    return ((dtype) std::rand() / (RAND_MAX));
}

bool vector4::operator==(const vector4& o) const {return x==o.x && y==o.y && z==o.z && w==o.w;}
bool vector4::operator!=(const vector4& o) const {return x!=o.x || y!=o.y || z!=o.z || w!=o.w;}

std::ostream& operator<< (std::ostream& os, const vector4& v)
{  
    os << '[' << v.x << ',' << v.y << ',' << v.z << ',' << v.w << ']';  
    return os;  
}

vector4& vector4::operator=(const vector4& o)
{
    x=o.x;y=o.y;z=o.z;w=o.w;
    return *this;
}
};
