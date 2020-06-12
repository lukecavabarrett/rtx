#ifndef _INCLUDE_RTX_vector2_H_
#define _INCLUDE_RTX_vector2_H_

#include <iostream>
#include "types.h"
namespace rtx {
class vector2 {

 public:
  dtype x, y;

  constexpr vector2();
  constexpr vector2(dtype v);
  constexpr vector2(dtype xp, dtype yp);
  constexpr vector2(const vector2 &o) = default;
  constexpr vector2(vector2 &&o) = default;
  constexpr dtype magnitude() const;
  constexpr dtype magnitude2() const;

  constexpr vector2 operator+(const vector2 &o) const;
  constexpr void operator+=(const vector2 &o);
  constexpr vector2 operator-(const vector2 &o) const;
  constexpr void operator-=(const vector2 &o);
  constexpr vector2 operator*(const vector2 &o) const;
  constexpr void operator*=(const vector2 &o);
  constexpr vector2 operator/(const vector2 &o) const;
  constexpr void operator/=(const vector2 &o);
  constexpr vector2 operator+(dtype o) const;
  constexpr void operator+=(dtype o);
  constexpr vector2 operator-(dtype o) const;
  constexpr vector2 operator-() const;
  constexpr void operator-=(dtype o);
  constexpr vector2 operator*(dtype o) const;
  constexpr void operator*=(dtype o);
  constexpr vector2 operator/(dtype o) const;
  constexpr void operator/=(dtype o);

  constexpr dtype distance(const vector2 &o) const;
  constexpr dtype distance_sqr(const vector2 &o) const;
  constexpr dtype dot(const vector2 &o) const;
  constexpr vector2 power(dtype o) const;
  constexpr vector2 inverse() const;
  constexpr vector2 relu() const;
  constexpr vector2 normalised() const;
  constexpr vector2 squared() const;
  constexpr void normalise();
  constexpr vector2 reflected_in(const vector2 &n) const;
  static vector2 random_inside_unit_circle();
  static dtype random_01();
  constexpr static vector2 add_until_normalized(const vector2 &a, const vector2 &b);
  constexpr bool operator==(const vector2 &o) const;
  constexpr bool operator!=(const vector2 &o) const;
  friend std::ostream &operator<<(std::ostream &os, const vector2 &v);
  constexpr vector2 &operator=(const vector2 &o);

  constexpr void rotate(dtype phi);

};
};
#include <cstdlib>
#include <cmath>
#include <cassert>

// constexpr implementation
namespace rtx {
constexpr vector2::vector2() : x(0), y(0) {}
constexpr vector2::vector2(dtype v) : x(v), y(v) {}
constexpr vector2::vector2(dtype xp, dtype yp) : x(xp), y(yp) {}

constexpr dtype vector2::magnitude() const { return std::sqrt(x * x + y * y ); }
constexpr dtype vector2::magnitude2() const { return x * x + y * y ; }

constexpr vector2 vector2::operator+(const vector2 &o) const { return vector2(x + o.x, y + o.y); }
constexpr void vector2::operator+=(const vector2 &o) {
  x += o.x;
  y += o.y;
}

constexpr vector2 vector2::operator-(const vector2 &o) const { return vector2(x - o.x, y - o.y); }
constexpr void vector2::operator-=(const vector2 &o) {
  x -= o.x;
  y -= o.y;
}
constexpr vector2 vector2::operator*(const vector2 &o) const { return vector2(x * o.x, y * o.y); }
constexpr void vector2::operator*=(const vector2 &o) {
  x *= o.x;
  y *= o.y;
}
constexpr vector2 vector2::operator/(const vector2 &o) const { return vector2(x / o.x, y / o.y); }
constexpr void vector2::operator/=(const vector2 &o) {
  x /= o.x;
  y /= o.y;
}
constexpr vector2 vector2::operator+(dtype o) const { return vector2(x + o, y + o); }
constexpr void vector2::operator+=(dtype o) {
  x += o;
  y += o;
}
constexpr vector2 vector2::operator-(dtype o) const { return vector2(x - o, y - o); }
constexpr vector2 vector2::operator-() const { return vector2(-x, -y); }
constexpr void vector2::operator-=(dtype o) {
  x -= o;
  y -= o;
}
constexpr vector2 vector2::operator*(dtype o) const { return vector2(x * o, y * o); }
constexpr void vector2::operator*=(dtype o) {
  x *= o;
  y *= o;
}
constexpr vector2 vector2::operator/(dtype o) const { return vector2(x / o, y / o); }
constexpr void vector2::operator/=(dtype o) {
  x /= o;
  y /= o;
}

constexpr dtype vector2::distance(const vector2 &o) const { return std::sqrt((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y)); }
constexpr dtype vector2::distance_sqr(const vector2 &o) const { return (x - o.x) * (x - o.x) + (y - o.y) * (y - o.y); }
constexpr dtype vector2::dot(const vector2 &o) const { return x * o.x + y * o.y; }
constexpr vector2 vector2::power(dtype o) const { return vector2(std::pow(x, o), std::pow(y, o)); }
constexpr vector2 vector2::inverse() const { return vector2(1 / x, 1 / y); }
constexpr vector2 vector2::relu() const { return vector2(x < 0 ? 0 : x, y < 0 ? 0 : y); }
constexpr vector2 vector2::normalised() const {
  dtype m = magnitude();
  return vector2(x / m, y / m);
}
constexpr void vector2::normalise() {
  dtype m = magnitude();
  x /= m;
  y /= m;
}

constexpr vector2 vector2::reflected_in(const vector2 &n) const {
  dtype d = dot(n) * 2;
  return vector2(d * n.x - x, d * n.y - y);
}


constexpr bool vector2::operator==(const vector2 &o) const { return x == o.x && y == o.y; }
constexpr bool vector2::operator!=(const vector2 &o) const { return x != o.x || y != o.y; }



constexpr vector2 &vector2::operator=(const vector2 &o) {
  x = o.x;
  y = o.y;
  return *this;
}
constexpr vector2 vector2::add_until_normalized(const vector2 &vc, const vector2 &va) {
  //
  dtype a = va.magnitude2(), b = 2 * va.dot(vc), c = vc.magnitude2() - 1.0;
  if (std::abs(a - 1.0) > eps)throw;
  dtype delta = b * b - 4 * a * c;
  if (delta < 0.0)throw;
  delta = std::sqrt(delta);
  dtype s1 = (-b + delta) / (2 * a), s2 = (-b - delta) / (2 * a);
  vector2 v = vc + va * std::max(s1, s2);
  if (std::abs(v.magnitude2() - 1.0) > eps)throw;
  return v.normalised();
}
constexpr void vector2::rotate(dtype phi) {
  dtype s = std::sin(phi), c=std::cos(phi);
  dtype nx = c*x-s*y;
  y = s*x + c*y;
  x = nx;
}
constexpr vector2 vector2::squared() const {
  return vector2(x * x, y * y);
}

};

#endif // _INCLUDE_RTX_vector2_H_
