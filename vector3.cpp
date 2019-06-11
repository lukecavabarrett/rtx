#include "vector3.h"
#include <cstdlib>
#include <cmath>
namespace rtx{
vector3::vector3() : x(0), y(0), z(0) {}
vector3::vector3(dtype v) : x(v), y(v), z(v) {}
vector3::vector3(dtype xp,dtype yp,dtype zp) : x(xp), y(yp), z(zp) {}
vector3::vector3(const vector3& o) : x(o.x), y(o.y), z(o.z) {}

dtype vector3::magnitude() const {return std::sqrt(x*x+y*y+z*z);}
vector3 vector3::operator+(const vector3& o) const {return vector3(x+o.x,y+o.y,z+o.z);}
void vector3::operator+=(const vector3& o){x+=o.x;y+=o.y;z+=o.z;}
vector3 vector3::operator-(const vector3& o) const {return vector3(x-o.x,y-o.y,z-o.z);}
void vector3::operator-=(const vector3& o){x-=o.x;y-=o.y;z-=o.z;}
vector3 vector3::operator*(const vector3& o) const {return vector3(x*o.x,y*o.y,z*o.z);}
void vector3::operator*=(const vector3& o){x*=o.x;y*=o.y;z*=o.z;}
vector3 vector3::operator/(const vector3& o) const {return vector3(x/o.x,y/o.y,z/o.z);}
void vector3::operator/=(const vector3& o){x/=o.x;y/=o.y;z/=o.z;}
vector3 vector3::operator+(dtype o) const {return vector3(x+o,y+o,z+o);}
void vector3::operator+=(dtype o){x+=o;y+=o;z+=o;}
vector3 vector3::operator-(dtype o) const {return vector3(x-o,y-o,z-o);}
vector3 vector3::operator-() const {return vector3(-x,-y,-z);}
void vector3::operator-=(dtype o){x-=o;y-=o;z-=o;}
vector3 vector3::operator*(dtype o) const {return vector3(x*o,y*o,z*o);}
void vector3::operator*=(dtype o){x*=o;y*=o;z*=o;}
vector3 vector3::operator/(dtype o) const {return vector3(x/o,y/o,z/o);}
void vector3::operator/=(dtype o){x/=o;y/=o;z/=o;}

dtype vector3::distance(const vector3& o) const {return std::sqrt((x-o.x)*(x-o.x)+(y-o.y)*(y-o.y)+(z-o.z)*(z-o.z));}
dtype vector3::distance_sqr(const vector3& o) const {return (x-o.x)*(x-o.x)+(y-o.y)*(y-o.y)+(z-o.z)*(z-o.z);}
dtype vector3::dot(const vector3& o) const {return x*o.x+y*o.y+z*o.z;}
vector3 vector3::cross(const vector3& o) const {return vector3(y*o.z-z*o.y,z*o.x-x*o.z,x*o.y-y*o.x);}
vector3 vector3::power(dtype o) const {return vector3(std::pow(x,o),std::pow(y,o),std::pow(z,o));}
vector3 vector3::inverse() const {return vector3(1/x,1/y,1/z);}
vector3 vector3::relu() const {return vector3(x<0?0:x,y<0?0:y,z<0?0:z);}
vector3 vector3::normalised() const {dtype m=magnitude();return vector3(x/m,y/m,z/m);}
void vector3::normalise(){dtype m=magnitude();x/=m;y/=m;z/=m;}

vector3 vector3::reflected_in(const vector3& n) const
{
    dtype d = dot(n)*2;
    return vector3(d*n.x-x,d*n.y-y,d*n.z-z);
}
vector3 vector3::random_inside_unit_sphere()
{
    dtype r = ((dtype) std::rand() / (RAND_MAX));
    dtype theta = ((dtype) std::rand() / (RAND_MAX)) * M_PIl;
    dtype phi = ((dtype) std::rand() / (RAND_MAX)) * M_PIl * 2;
    return vector3(r*std::sin(theta)*std::cos(phi),
                    r*std::sin(theta)*std::sin(phi),
                    r*std::cos(theta));
}
dtype vector3::random_01()
{
    return ((dtype) std::rand() / (RAND_MAX));
}

bool vector3::operator==(const vector3& o) const {return x==o.x && y==o.y && z==o.z;}
bool vector3::operator!=(const vector3& o) const {return x!=o.x || y!=o.y || z!=o.z;}

std::ostream& operator<< (std::ostream& os, const vector3& v)
{  
    os << '[' << v.x << ',' << v.y << ',' << v.z << ']';  
    return os;  
}

vector3& vector3::operator=(const vector3& o)
{
    x=o.x;y=o.y;z=o.z;
    return *this;
}
};
