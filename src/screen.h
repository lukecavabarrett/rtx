#ifndef _INCLUDE_SCREEN_H_
#define _INCLUDE_SCREEN_H_

#include <X11/Xlib.h> // must precede most other headers!
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <cstdio>
#include <X11/Xutil.h>
#include <iostream>
#include "typedefs.h"

namespace rtx{
    class screen{
    unsigned int xres,yres;
    int scr;
    unsigned int whiteColor,blackColor;
    Display *dpy;
    Window w;
    GC gc;
    XImage *ximage;
    Atom wmDelete;
    public:
    screen(int width,int height);
    void put_pixel(int y,int x,int c);
    void wait_key(const char* str);
    int color_black() const ;
    int color_white()const ;
    int size_y() const ;
    int size_x()const ;
    ~screen();
    };
};
#endif
