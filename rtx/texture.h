#ifndef SAMPLE_SRC_TEXTURE_H_
#define SAMPLE_SRC_TEXTURE_H_

#include "types.h"

namespace rtx{
class texture{
 public:
  //color_rgb get_color(const vector3& v) const;
};

class color_rgb;

class grid_texture : public texture {
 public:
  color_rgb get_color(const vector3& v) const ;
  grid_texture(dtype m = 1,dtype s =1) : m(m),scale(s) {};
 private:
  dtype m = 1, scale = 1;
};

};

#endif //SAMPLE_SRC_TEXTURE_H_
