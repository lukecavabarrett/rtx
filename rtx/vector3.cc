#include "vector3.h"

namespace rtx{

std::ostream &operator<<(std::ostream &os, const vector3 &v) {
  os << '[' << v.x << ',' << v.y << ',' << v.z << ']';
  return os;
}

vector3 vector3::random_inside_unit_sphere() {
  dtype r = ((dtype) std::rand() / (RAND_MAX));
  dtype theta = ((dtype) std::rand() / (RAND_MAX)) * M_PIl;
  dtype phi = ((dtype) std::rand() / (RAND_MAX)) * M_PIl * 2;
  return vector3(r * std::sin(theta) * std::cos(phi),
                 r * std::sin(theta) * std::sin(phi),
                 r * std::cos(theta));
}
dtype vector3::random_01() {
  return ((dtype) std::rand() / (RAND_MAX));
}
}