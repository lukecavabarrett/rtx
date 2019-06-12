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
#include "vector3.h"
#include "color_rgb.h"
#include "ray3.h"

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
    /*screen(int width,int height) : xres(width), yres(height), dsp(XOpenDisplay(NULL)), scr(DefaultScreen(dsp)), white(WhitePixel(dsp,scr)), black(BlackPixel(dsp,scr)), win(XCreateSimpleWindow(dsp,DefaultRootWindow(dsp),0, 0,xres, yres,0, black,white)), wmDelete(XInternAtom(dsp, "WM_DELETE_WINDOW", True)) {
        XSetWMProtocols(dsp, win, &wmDelete, 1);
        gc = XCreateGC(dsp, win,0,NULL);
        XSetForeground(dsp, gc, black);
    }*/
    screen(int width,int height) : xres(width), yres(height){
        dpy = XOpenDisplay(NULL);
        if(!dpy)exit(1);

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

      for(;;) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    if (e.type == MapNotify)
		  break;
      }
        
    }
    void put_pixel(int y,int x,int c) {
        XSetForeground(dpy, gc, c);
        XDrawPoint(dpy,w,gc,x,y);
        //XFlush(dpy);
    }
    void wait_key(const char* str) {
        XSelectInput(dpy, w, KeyReleaseMask);
        KeyCode keyQ;
        keyQ = XKeysymToKeycode(dpy, XStringToKeysym(str));
        for(;;) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    if (e.xkey.keycode == keyQ)
		  break;
      }
    }
    int color_black(){return blackColor;}
    int color_white(){return whiteColor;}
    int size_y(){return yres;}
    int size_x(){return xres;}
    ~screen() {
       //wait to be closed
        
    }
    /*
    int screen = DefaultScreen(dsp);
    unsigned int white = WhitePixel(dsp,screen);
    unsigned int black = BlackPixel(dsp,screen);

    win = XCreateSimpleWindow(dsp,DefaultRootWindow(dsp),0, 0,XRES, YRES,0, black,white);

    Atom wmDelete=XInternAtom(dsp, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(dsp, win, &wmDelete, 1);
    gc = XCreateGC(dsp, win,0,NULL);
    XSetForeground(dsp, gc, black);
    XEvent evt;
    long eventMask = StructureNotifyMask;
    eventMask |= ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask;
    XSelectInput(dsp, win, eventMask);
    
    Visual *visual = DefaultVisual(dsp, 0);
    ximage = XCreateImage(dsp, visual, 24,ZPixmap, 0, (char *)camera.video_buffer,XRES, YRES, 32, 0);

    KeyCode keyQ;
    keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));

    XMapWindow(dsp, win);

    // wait until window appears
    do { XNextEvent(dsp,&evt); } while (evt.type != MapNotify);
    

    re.render(scene,camera,ambient,background,sun);
    XPutImage(dsp, win,gc, ximage, 0, 0, 0, 0,XRES, YRES);
    */
    };
};
#endif
