
#include "rtx.h"
namespace rtx{

inline double mfmod(double x,double r){
  x = std::fmod(x,r);
  if(x<0)x+=r;
  return x;
}

color_rgb grid_texture::get_color(const vector3 &v) const {
  //return color_rgb(std::abs(v.y));
  return color_rgb(m*int((mfmod(v.x,scale*2)<scale)==(mfmod(v.y,scale*2)<scale)));
}

}