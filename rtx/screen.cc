#include "rtx.h"
#include "screen.h"

#include <cstdlib>
#include <cmath>
#include <limits>

namespace rtx {
screen::screen(int width, int height, rgb_color c) {
  width_ = width;
  height_ = height;
  display_ = XOpenDisplay(NULL);
  if (!display_)exit(1);

  window_ = XCreateSimpleWindow(display_, DefaultRootWindow(display_), 0, 0,
                                width_, height_, 0, 0, 0);

  XSelectInput(display_, window_, StructureNotifyMask);

  XMapWindow(display_, window_);

  gc_ = XCreateGC(display_, window_, 0, NULL);

  XSetForeground(display_, gc_, c.data);



  for (;;) {
    XEvent e;
    XNextEvent(display_, &e);
    if (e.type == MapNotify)
      break;
  }

  XFillRectangle(display_, window_, gc_, 0, 0, width, height);


}

void screen::resize(int width,int height){
  XResizeWindow(display_,window_,width_=width,height_=height);
}

void screen::put_pixel(int x, int y, rgb_color c) {
  XSetForeground(display_, gc_, c.data);
  XDrawPoint(display_, window_, gc_, x, y);
  //XFlush(display_);
}
void screen::put_rectangle(int x, int y, int wi, int h, rgb_color c, bool fill) {

  XSetForeground(display_, gc_, c.data);
  //for(int i=x;i<x+wi;++i)for(int j=y;j<y+h;++j)XDrawPoint(display_,w,gc,i,j);
  if(fill)XFillRectangle(display_, window_, gc_, x, y, wi, h);
  else XDrawRectangle(display_, window_, gc_, x, y, wi, h);
  //XFlush(display_);
}

void screen::flush(){
  XFlush(display_);
}

void screen::wait_key(const char *str) {
  XSelectInput(display_, window_, KeyReleaseMask);
  KeyCode keyQ;
  keyQ = XKeysymToKeycode(display_, XStringToKeysym(str));
  for (;;) {
    XEvent e;
    XNextEvent(display_, &e);
    if (e.xkey.keycode == keyQ)
      break;
  }
}
void screen::wait_key(int keyQ) {
  XSelectInput(display_, window_, KeyReleaseMask);
  for (;;) {
    XEvent e;
    XNextEvent(display_, &e);
    if (e.xkey.keycode == keyQ)
      break;
  }
}
int screen::wait_key() {
  XSelectInput(display_, window_, KeyPressMask);
  XEvent e;
  XNextEvent(display_, &e);
  return e.xkey.keycode;
}

int screen::height() const { return height_; }
int screen::width() const { return width_; }
screen::~screen() {
  //wait to be closed
  XCloseDisplay(display_);
}
void screen::save_image(const char* path) {

}

};
