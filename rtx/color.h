#ifndef _INCLUDE_RTX_COLOR_H_
#define	_INCLUDE_RTX_COLOR_H_

#include "vector3.h"
#include <iostream>
#include <algorithm>
namespace rtx{

class color;

template<uint64_t ChannelDepth,bool LittleEndian=false>
struct basic_rgb_color{
  static constexpr uint64_t channel_depth = ChannelDepth;
  static constexpr uint64_t channel_space = (1ULL<<channel_depth);
  static constexpr uint64_t channel_max = channel_space-1;
  static_assert(channel_depth*3<=64);
  static constexpr uint64_t bits_size = channel_depth*3;
  typedef std::conditional_t<bits_size<=8,uint8_t,std::conditional_t<bits_size<=16,uint16_t,std::conditional_t<bits_size<=32,uint32_t,std::conditional_t<bits_size<=64,uint64_t,void>>>> field_t;
  field_t data;
 private:
  //TODO: add conversions from wider/smaller types

  static constexpr inline field_t dtype_to_integral(dtype x){
    return std::min<field_t>(static_cast<field_t>(x*channel_max),channel_max);
  }

  static constexpr inline field_t from_color(const color& c);

  static constexpr inline dtype integral_to_dtype(field_t x){
    return static_cast<dtype>(x)/channel_max;
  }



 public:
  constexpr basic_rgb_color() : data(0) {}
  constexpr basic_rgb_color(field_t c) : data(c) {}
  constexpr basic_rgb_color(const basic_rgb_color&) = default;
  constexpr basic_rgb_color(basic_rgb_color&&) = default;
  constexpr basic_rgb_color(const color&) {

  }
  constexpr basic_rgb_color& operator=(const basic_rgb_color&) noexcept = default;
  constexpr basic_rgb_color& operator=(basic_rgb_color&&)  noexcept = default;
};



typedef basic_rgb_color<8,false> rgb_color;
constexpr rgb_color operator""_rgb(unsigned long long c){
  return static_cast<rgb_color>(static_cast<rgb_color::field_t>(c));
}

class color : public vector3
{
public:
    using vector3::vector3;
private:
//    static int to_byte(dtype v);
public:
//    color_rgb (const vector3& v);
    //color_rgb (dtype r,dtype g,dtype b);
//    int to_rgb() const;
//    static color_rgb from_rgb(int w);
//    color_rgb get_color(const vector3& v) const;
//    int to_rgb_le() const;
//    const vector3& to_vec() const;
//    color_rgb tonemap() const;
 //   friend std::ostream& operator<<(std::ostream& os, const color_rgb& v);
};

namespace tonemapping {
  static constexpr dtype inv_gamma = 1.0/2.2;
  static constexpr dtype g_a = 2.0;
  static constexpr dtype g_b = 1.3;
};

};

//constexpr implementation
namespace rtx{
template<uint64_t Depth,bool LittleEndian>
 constexpr inline typename basic_rgb_color<Depth,LittleEndian>::field_t basic_rgb_color<Depth,LittleEndian>::from_color(const color& c)  {
  if constexpr (LittleEndian){
    return (dtype_to_integral(c.z)<<(channel_depth*2)) | (dtype_to_integral(c.y)<<channel_depth) | dtype_to_integral(c.x); ;
  } else {
    return (dtype_to_integral(c.x)<<(channel_depth*2)) | (dtype_to_integral(c.y)<<channel_depth) | dtype_to_integral(c.z);
  }
}
}
#endif // _INCLUDE_RTX_COLOR_H_
