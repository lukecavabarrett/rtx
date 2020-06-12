#include "vector2.h"
namespace rtx {
vector2 vector2::random_inside_unit_circle() {
  dtype r = ((dtype) std::rand() / (RAND_MAX));
  dtype theta = ((dtype) std::rand() / (RAND_MAX)) * M_PIl;
  return vector2(r * std::cos(theta), r * std::sin(theta));
}
dtype vector2::random_01() {
  return ((dtype) std::rand() / (RAND_MAX));
}
std::ostream &operator<<(std::ostream &os, const vector2 &v) {
  os << '[' << v.x << ',' << v.y << ','  << ']';
  return os;
}
}