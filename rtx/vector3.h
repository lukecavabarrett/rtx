#ifndef _INCLUDE_RTX_VECTOR3_H_
#define _INCLUDE_RTX_VECTOR3_H_

#include <iostream>
#include "types.h"
namespace rtx {
class vector3 {

 public:
  dtype x, y, z;

  constexpr vector3();
  constexpr vector3(dtype v);
  constexpr vector3(dtype xp, dtype yp, dtype zp);
  constexpr vector3(const vector3 &o) = default;
  constexpr vector3(vector3 &&o) = default;
  constexpr dtype magnitude() const;
  constexpr dtype magnitude2() const;

  constexpr vector3 operator+(const vector3 &o) const;
  constexpr void operator+=(const vector3 &o);
  constexpr vector3 operator-(const vector3 &o) const;
  constexpr void operator-=(const vector3 &o);
  constexpr vector3 operator*(const vector3 &o) const;
  constexpr void operator*=(const vector3 &o);
  constexpr vector3 operator/(const vector3 &o) const;
  constexpr void operator/=(const vector3 &o);
  constexpr vector3 operator+(dtype o) const;
  constexpr void operator+=(dtype o);
  constexpr vector3 operator-(dtype o) const;
  constexpr vector3 operator-() const;
  constexpr void operator-=(dtype o);
  constexpr vector3 operator*(dtype o) const;
  constexpr void operator*=(dtype o);
  constexpr vector3 operator/(dtype o) const;
  constexpr void operator/=(dtype o);

  constexpr dtype distance(const vector3 &o) const;
  constexpr dtype distance_sqr(const vector3 &o) const;
  constexpr dtype dot(const vector3 &o) const;
  constexpr vector3 cross(const vector3 &o) const;
  constexpr vector3 power(dtype o) const;
  constexpr vector3 inverse() const;
  constexpr vector3 relu() const;
  constexpr vector3 normalised() const;
  constexpr vector3 squared() const;
  constexpr void normalise();
  constexpr vector3 reflected_in(const vector3 &n) const;
  static vector3 random_inside_unit_sphere();
  static dtype random_01();
  constexpr static vector3 add_until_normalized(const vector3 &a, const vector3 &b);
  constexpr bool operator==(const vector3 &o) const;
  constexpr bool operator!=(const vector3 &o) const;
  friend std::ostream &operator<<(std::ostream &os, const vector3 &v);
  constexpr vector3 &operator=(const vector3 &o);

  constexpr void rotate(vector3 o, dtype phi);

};
};
#include <cstdlib>
#include <cmath>
#include <cassert>
// constexpr implementation
namespace rtx {
constexpr vector3::vector3() : x(0), y(0), z(0) {}
constexpr vector3::vector3(dtype v) : x(v), y(v), z(v) {}
constexpr vector3::vector3(dtype xp, dtype yp, dtype zp) : x(xp), y(yp), z(zp) {}

constexpr dtype vector3::magnitude() const { return std::sqrt(x * x + y * y + z * z); }
constexpr dtype vector3::magnitude2() const { return x * x + y * y + z * z; }

constexpr vector3 vector3::operator+(const vector3 &o) const { return vector3(x + o.x, y + o.y, z + o.z); }
constexpr void vector3::operator+=(const vector3 &o) {
  x += o.x;
  y += o.y;
  z += o.z;
}

constexpr vector3 vector3::operator-(const vector3 &o) const { return vector3(x - o.x, y - o.y, z - o.z); }
constexpr void vector3::operator-=(const vector3 &o) {
  x -= o.x;
  y -= o.y;
  z -= o.z;
}
constexpr vector3 vector3::operator*(const vector3 &o) const { return vector3(x * o.x, y * o.y, z * o.z); }
constexpr void vector3::operator*=(const vector3 &o) {
  x *= o.x;
  y *= o.y;
  z *= o.z;
}
constexpr vector3 vector3::operator/(const vector3 &o) const { return vector3(x / o.x, y / o.y, z / o.z); }
constexpr void vector3::operator/=(const vector3 &o) {
  x /= o.x;
  y /= o.y;
  z /= o.z;
}
constexpr vector3 vector3::operator+(dtype o) const { return vector3(x + o, y + o, z + o); }
constexpr void vector3::operator+=(dtype o) {
  x += o;
  y += o;
  z += o;
}
constexpr vector3 vector3::operator-(dtype o) const { return vector3(x - o, y - o, z - o); }
constexpr vector3 vector3::operator-() const { return vector3(-x, -y, -z); }
constexpr void vector3::operator-=(dtype o) {
  x -= o;
  y -= o;
  z -= o;
}
constexpr vector3 vector3::operator*(dtype o) const { return vector3(x * o, y * o, z * o); }
constexpr void vector3::operator*=(dtype o) {
  x *= o;
  y *= o;
  z *= o;
}
constexpr vector3 vector3::operator/(dtype o) const { return vector3(x / o, y / o, z / o); }
constexpr void vector3::operator/=(dtype o) {
  x /= o;
  y /= o;
  z /= o;
}

constexpr dtype vector3::distance(const vector3 &o) const { return std::sqrt((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y) + (z - o.z) * (z - o.z)); }
constexpr dtype vector3::distance_sqr(const vector3 &o) const { return (x - o.x) * (x - o.x) + (y - o.y) * (y - o.y) + (z - o.z) * (z - o.z); }
constexpr dtype vector3::dot(const vector3 &o) const { return x * o.x + y * o.y + z * o.z; }
constexpr vector3 vector3::cross(const vector3 &o) const { return vector3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x); }
constexpr vector3 vector3::power(dtype o) const { return vector3(std::pow(x, o), std::pow(y, o), std::pow(z, o)); }
constexpr vector3 vector3::inverse() const { return vector3(1 / x, 1 / y, 1 / z); }
constexpr vector3 vector3::relu() const { return vector3(x < 0 ? 0 : x, y < 0 ? 0 : y, z < 0 ? 0 : z); }
constexpr vector3 vector3::normalised() const {
  dtype m = magnitude();
  return vector3(x / m, y / m, z / m);
}
constexpr void vector3::normalise() {
  dtype m = magnitude();
  x /= m;
  y /= m;
  z /= m;
}

constexpr vector3 vector3::reflected_in(const vector3 &n) const {
  dtype d = dot(n) * 2;
  return vector3(d * n.x - x, d * n.y - y, d * n.z - z);
}


constexpr bool vector3::operator==(const vector3 &o) const { return x == o.x && y == o.y && z == o.z; }
constexpr bool vector3::operator!=(const vector3 &o) const { return x != o.x || y != o.y || z != o.z; }



constexpr vector3 &vector3::operator=(const vector3 &o) {
  x = o.x;
  y = o.y;
  z = o.z;
  return *this;
}
constexpr vector3 vector3::add_until_normalized(const vector3 &vc, const vector3 &va) {
  //
  dtype a = va.magnitude2(), b = 2 * va.dot(vc), c = vc.magnitude2() - 1.0;
  if (std::abs(a - 1.0) > eps)throw;
  dtype delta = b * b - 4 * a * c;
  if (delta < 0.0)throw;
  delta = std::sqrt(delta);
  dtype s1 = (-b + delta) / (2 * a), s2 = (-b - delta) / (2 * a);
  vector3 v = vc + va * std::max(s1, s2);
  if (std::abs(v.magnitude2() - 1.0) > eps)throw;
  return v.normalised();
}
constexpr void vector3::rotate(vector3 o, dtype phi) {
  o.normalise();
  dtype p = dot(o);
  operator-=(o * p);
  vector3 d = cross(o);
  operator*=(std::cos(phi));
  operator+=(d * std::sin(phi));
  operator+=(o * p);
}
constexpr vector3 vector3::squared() const {
  return vector3(x * x, y * y, z * z);
}

};

#endif // _INCLUDE_RTX_VECTOR3_H_
