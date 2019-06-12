#ifndef _INCLUDE_VECTOR3_H_
#define _INCLUDE_VECTOR3_H_

#include <iostream>
#include "typedefs.h"
namespace rtx{
class vector3
{
    
public:
    dtype x,y,z;
    vector3();
    vector3(dtype v);
    vector3(dtype xp,dtype yp,dtype zp);
    vector3(const vector3& o);
    dtype magnitude() const;
    
    vector3 operator+(const vector3& o) const ;
    void operator+=(const vector3& o);
    vector3 operator-(const vector3& o) const ;
    void operator-=(const vector3& o);
    vector3 operator*(const vector3& o) const ;
    void operator*=(const vector3& o);
    vector3 operator/(const vector3& o) const ;
    void operator/=(const vector3& o);
    vector3 operator+(dtype o) const ;
    void operator+=(dtype o);
    vector3 operator-(dtype o) const ;
    vector3 operator-() const ;
    void operator-=(dtype o);
    vector3 operator*(dtype o) const ;
    void operator*=(dtype o);
    vector3 operator/(dtype o) const ;
    void operator/=(dtype o);
    
    dtype distance(const vector3& o) const ;
    dtype distance_sqr(const vector3& o) const ;
    dtype dot(const vector3& o) const ;
    vector3 cross(const vector3& o) const ;
    vector3 power(dtype o) const ;
    vector3 inverse() const ;
    vector3 relu() const ;
    vector3 normalised() const ;
    void normalise();
    vector3 reflected_in(const vector3& n) const;
    static vector3 random_inside_unit_sphere();
    static dtype random_01();
    bool operator==(const vector3& o) const;
    bool operator!=(const vector3& o) const;
    friend std::ostream& operator<<(std::ostream& os, const vector3& v);
    vector3& operator=(const vector3& o);
    
};
};
#endif
