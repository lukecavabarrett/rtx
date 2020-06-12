#include "rtx.h"
#include "sphere.h"

#include <cstdlib>
#include <cmath>
#include <limits>

namespace rtx {
sphere::sphere(const vector3 &p, dtype r) : position(p), radius(r) {}
vector3 sphere::get_normal_at(const vector3 &v) const {
  return (v - position).normalised();
}
vector3 sphere::get_mapping_at(const vector3 &v) const {
  vector3 pu(1, 0, 0);
  vector3 n = get_normal_at(v);
  vector3 pv = n.cross(pu);
  return vector3((std::atan2(n.z, n.x) + M_PIl) / (M_PIl * 2.0), (1.0 - n.y) / 2, v.z);
}
std::pair<dtype, vector3> sphere::intersection_with(const ray3 &r) const {
  dtype a = r.d.magnitude2();
  dtype b = 2 * r.d.dot(r.o - position);
  dtype c = (r.o - position).dot(r.o - position) - radius * radius;
  dtype delta = b * b - 4 * a * c;
  if (delta < 0)return std::pair<dtype, vector3>(std::numeric_limits<dtype>::infinity(), vector3(0));
  delta = std::sqrt(delta);
  dtype s = -b;
  if (s + delta < 0)return std::pair<dtype, vector3>(std::numeric_limits<dtype>::infinity(), vector3(0));
  if (s - delta < 0)s += delta; else s -= delta;
  s /= (2 * a);
  vector3 p = r.evaluate_at(s);
  return std::pair<dtype, vector3>(s, p);
}
color_rgb sphere::trace(const vector3 &p, const ray3 &r, const scene &s, int bounces) const { return color_rgb(0); }

cylinder::cylinder(const vector3 &p, const vector3 &v, dtype r) : position(p), vertical(v), radius(r)  {}

vector3 cylinder::get_normal_at(const vector3 &v) const {
  vector3 w = v;
  w -= position;
  w -= vertical*w.dot(vertical)/vertical.magnitude2();
  return w.normalised();
}
vector3 cylinder::get_mapping_at(const vector3 &v) const {

}
std::pair<dtype, vector3> cylinder::intersection_with(const ray3 &r) const {
  vector3 d = r.d,o=r.o;
  o -= position;
  o -= vertical*o.dot(vertical)/vertical.magnitude2();
  d -= vertical*d.dot(vertical)/vertical.magnitude2();
  dtype a = d.magnitude2();
  dtype b = 2 * d.dot(o);
  dtype c = o.magnitude2() - radius*radius;
  dtype delta = b * b - 4 * a * c;
  if (delta < 0)return std::pair<dtype, vector3>(std::numeric_limits<dtype>::infinity(), vector3(0));
  delta = std::sqrt(delta)/(2*a);
  dtype s = -b / (2*a);
  if (s + delta < 0)return std::pair<dtype, vector3>(std::numeric_limits<dtype>::infinity(), vector3(0));

//TODO: avoid normalising vertical
  if(s-delta>=0) {
    vector3 p = r.evaluate_at(s-delta);
    dtype x = vertical.normalised().dot(p - position);
    if (x >= 0 && x <= vertical.magnitude())
      return std::pair<dtype, vector3>(s-delta, p);
  }
  vector3 p = r.evaluate_at(s+delta);
  dtype x = vertical.normalised().dot(p - position);
  if (x >= 0 && x <= vertical.magnitude())
    return std::pair<dtype, vector3>(s+delta, p);
  return std::pair<dtype, vector3>(std::numeric_limits<dtype>::infinity(), vector3(0));
}
color_rgb cylinder::trace(const vector3 &p, const ray3 &r, const scene &s, int bounces) const { return color_rgb(0); }


};
