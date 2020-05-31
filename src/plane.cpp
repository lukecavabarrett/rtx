#include "rtx.h"
#include <cstdlib>
#include <cmath>
#include <limits>

namespace rtx {
plane::plane(const vector3 &p, const vector3 &n) : position(p), normal(n.normalised()) {}
vector3 plane::get_normal_at(const vector3 &v) const {
  return normal;
}
vector3 plane::get_mapping_at(const vector3 &v) const {
  return vector3(v.x+v.y,v.y+v.z,0);
  auto w = v - position;
  w -= w.dot(normal);
  return w;
}
std::pair<dtype, vector3> plane::intersection_with(const ray3 &r) const {
  dtype den = normal.dot(r.d);
  if (den == 0)return std::pair<dtype, vector3>(std::numeric_limits<dtype>::infinity(), vector3(0));
  dtype s = -normal.dot(r.o - position) / den;
  if (s <= 0)return std::pair<dtype, vector3>(std::numeric_limits<dtype>::infinity(), vector3(0));
  vector3 p = r.evaluate_at(s);
  return std::pair<dtype, vector3>(s, p);
}
color_rgb plane::trace(const vector3 &p, const ray3 &r, const scene &s, int bounces) const { return color_rgb(0); }

};
