#include "rtx.h"
#include "screen.h"

#include <cstdlib>
#include <cmath>
#include <limits>

namespace rtx {
screen::screen(int width, int height) {
  xres = width;
  yres = height;
  dpy = XOpenDisplay(NULL);
  if (!dpy)exit(1);

  // Get some colors

  blackColor = BlackPixel(dpy, DefaultScreen(dpy));
  whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

  // Create the window

  w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
                          xres, yres, 0, blackColor, blackColor);

  // We want to get MapNotify events

  XSelectInput(dpy, w, StructureNotifyMask);

  // "Map" the window (that is, make it appear on the screen)

  XMapWindow(dpy, w);

  // Create a "Graphics Context"

  gc = XCreateGC(dpy, w, 0, NULL);

  // Tell the GC we draw using the white color

  XSetForeground(dpy, gc, whiteColor);

  // Wait for the MapNotify event

  for (;;) {
    XEvent e;
    XNextEvent(dpy, &e);
    if (e.type == MapNotify)
      break;
  }

}
void screen::put_pixel(int x, int y, int c) {
  XSetForeground(dpy, gc, c);
  XDrawPoint(dpy, w, gc, x, y);
  //XFlush(dpy);
}
void screen::put_rectangle(int x, int y,int wi,int h, int c,bool fill) {

  XSetForeground(dpy, gc, c);
  //for(int i=x;i<x+wi;++i)for(int j=y;j<y+h;++j)XDrawPoint(dpy,w,gc,i,j);
  if(fill)XFillRectangle(dpy,w,gc,x,y,wi,h);
  else XDrawRectangle(dpy,w,gc,x,y,wi,h);
  //XFlush(dpy);
}

void screen::flush(){
  XFlush(dpy);
}

void screen::wait_key(const char *str) {
  XSelectInput(dpy, w, KeyReleaseMask);
  KeyCode keyQ;
  keyQ = XKeysymToKeycode(dpy, XStringToKeysym(str));
  for (;;) {
    XEvent e;
    XNextEvent(dpy, &e);
    if (e.xkey.keycode == keyQ)
      break;
  }
}
int screen::wait_key() {
  XSelectInput(dpy, w, KeyPressMask);
  XEvent e;
  XNextEvent(dpy, &e);
  return e.xkey.keycode;
}
int screen::color_black() const { return blackColor; }
int screen::color_white() const { return whiteColor; }
int screen::size_y() const { return yres; }
int screen::size_x() const { return xres; }
screen::~screen() {
  //wait to be closed

}

};
