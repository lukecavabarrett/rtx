#ifndef _INCLUDE_RTX_SCREEN_H_
#define _INCLUDE_RTX_SCREEN_H_

#include <X11/Xlib.h> // must precede most other headers!
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <cstdio>
#include <X11/Xutil.h>
#include <iostream>
#include "color.h"

namespace rtx {
class screen {
  unsigned int width_, height_;
  Display *display_;
  Window window_;
  GC gc_;
 public:
  screen(int width, int height, rgb_color c = 0x000000_rgb);
  void save_image(const char *path);
  void resize(int width,int height);
  void put_pixel(int x, int y, rgb_color c);
  void put_rectangle(int x, int y, int wi, int h, rgb_color c, bool fill = true);
  void flush();
  void wait_key(const char *str);
  void wait_key(int);
  int wait_key();
  int height() const;
  int width() const;
  ~screen();
};
};
#endif //_INCLUDE_RTX_SCREEN_H_
