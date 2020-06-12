#ifndef _INCLUDE_TRANSPARENT_H_
#define _INCLUDE_TRANSPARENT_H_

#include <iostream>
#include "rtx.h"

namespace rtx {
template<typename T>
class transparent : public scene_object {
  static_assert(std::is_base_of<scene_object, T>::value, "T must derive from scene_object");
 private:
  T internal;
  dtype rate, koeff, alpha;

 public:

  template<typename... Ts>
  transparent(dtype r, dtype k, dtype a, Ts &&... args) : internal(std::forward<Ts>(args)...), rate(std::min<dtype>(std::max<dtype>(0, r), 1)), koeff(k),alpha(a) {};

  virtual vector3 get_normal_at(const vector3 &v) const override {
    return internal.get_normal_at(v);
  }

  virtual vector3 get_mapping_at(const vector3 &v) const override {
    return internal.get_mapping_at(v);
  }

  virtual std::pair<dtype, vector3> intersection_with(const ray3 &r) const override {
    return internal.intersection_with(r);
  }

  virtual color_rgb trace(const vector3 &p, const ray3 &r, const scene &s, int bounces = 0) const override {

    if (bounces) {
      vector3 n = get_normal_at(p);
      dtype k = koeff;
      if (n.dot(r.d) > 0) {
        n = -n;
        k = dtype(1) / k;
      }
      vector3 vs = (r.d - n * (n.dot(r.d))) * k;
      if (vs.magnitude2() > 1.0) {
        return internal.trace(p, r, s, bounces);
        vector3 vr = r.d - n * (n.dot(r.d)) * 2;
        vr.normalise();
        ray3 ref(p + vr * eps, vr);
        return internal.trace(p, r, s, bounces) * (1 - rate) + s.trace(ref, bounces - 1) * rate;
      }
      dtype adjrate = rate * (1.0f - std::pow(vs.magnitude2(),alpha/2));
      vector3 vr = vector3::add_until_normalized(vs, -n);
      ray3 ref(p + vr * eps, vr);
      if(alpha>40)return s.trace(ref, bounces - 1);
      return internal.trace(p, r, s, bounces) * (1 - adjrate) + s.trace(ref, bounces - 1) * adjrate;
    } else {
      return internal.trace(p, r, s, bounces);
    }
  }

  virtual uint64_t instance_id() const override {
    return internal.instance_id();
  }

};


template<typename T>
class bumped : public scene_object {
  static_assert(std::is_base_of<scene_object, T>::value, "T must derive from scene_object");
 private:
  T internal;
  dtype scale, height;

 public:

  template<typename... Ts>
  bumped(dtype s, dtype h, Ts &&... args) : internal(std::forward<Ts>(args)...), scale(s), height(h) {};

  virtual vector3 get_normal_at(const vector3 &v) const override {
    auto m = internal.get_mapping_at(v), n = internal.get_normal_at(v);
    auto v1 = n.cross(n.cross(vector3(1,0,0))+n.cross(vector3(0,1,0))).normalised();
    auto v2 = n.cross(v1).normalised();
    //vector3 v1(1,0,0),v2(0,0,1);
    //return (n+v1*height).normalised();
    return (n + v1*std::pow(std::sin(m.x*scale),1)*height + v2*std::pow(std::cos(m.y*scale),1)*height).normalised();
  }

  virtual vector3 get_mapping_at(const vector3 &v) const override {
    return internal.get_mapping_at(v);
  }

  virtual std::pair<dtype, vector3> intersection_with(const ray3 &r) const override {
    return internal.intersection_with(r);
  }

  virtual color_rgb trace(const vector3 &p, const ray3 &r, const scene &s, int bounces = 0) const override {
      return internal.trace(p, r, s, bounces);
  }

  virtual uint64_t instance_id() const override {
    return internal.instance_id();
  }

};
};
#endif
