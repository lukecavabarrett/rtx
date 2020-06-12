#ifndef _INCLUDE_PLANE_H_
#define _INCLUDE_PLANE_H_

#include <iostream>
#include "types.h"

namespace rtx{
class plane : public scene_object {
 private:
  vector3 normal;
 protected:
  vector3 position;
 public:
  plane(const vector3& p,const vector3& n);
  virtual vector3 get_normal_at(const vector3& v) const override;
  virtual vector3 get_mapping_at(const vector3& v) const override;
  virtual std::pair<dtype,vector3> intersection_with(const ray3& r) const override;
  virtual color_rgb trace(const vector3& p,const ray3& r,const scene& s,int bounces=0) const override;
};

class disk : public plane {
 private:
  dtype radius;
 public:
  disk(const vector3& p,const vector3& n,const dtype r);
  using plane::get_normal_at;
  using plane::get_mapping_at;
  using plane::trace;
  virtual std::pair<dtype,vector3> intersection_with(const ray3& r) const override;
};

class disk_holed : public disk {
 private:
  dtype inner_radius;
 public:
  disk_holed(const vector3& p,const vector3& n,const dtype r, const dtype r2);
  using plane::get_normal_at;
  using plane::get_mapping_at;
  using plane::trace;
  virtual std::pair<dtype,vector3> intersection_with(const ray3& r) const override;
};

};
#endif
