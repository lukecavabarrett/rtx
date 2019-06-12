#include "src/rtx.h"
//#include <bits/stdc++.h>

/*


//abstract class
class obj{
public:
    //does nothing
    virtual int f()=0;
};

//two concrete classes: concA just return the value, concB the number of bits of the value

class concA : public obj {
    int x;
public:
    concA(int w): x(w) {}
    concA(int decine,int unita){ x = 10*decine+(unita%10);}
    int f() {return x;}
};

class concB : public obj {
    int x;
public:
    concB(int w): x(w) {}
    int f() {return __builtin_popcount(x);}
};

//two decorator: decA add a value, decB multiply by a fixed value
template<typename T>
class decA : public obj {
static_assert(std::is_base_of<obj, T>::value, "T must derive from obj");

T internal;
int add;
public:
template <typename... Ts>
decA(int a,Ts&&... args) : internal(std::forward<Ts>(args)...),add(a) {}

int f() {return internal.f()+add;}

};

template<typename T>
class decB : public obj {
static_assert(std::is_base_of<obj, T>::value, "T must derive from obj");

T internal;
int mul;
public:
template <typename... Ts>
decB(int m,Ts&&... args);

int f() {return internal.f()*mul;}

};

template<typename T>
template<typename... Ts>
decB<T>::decB(int m,Ts&&... args)  : internal(std::forward<Ts>(args)...),mul(m) {}



int main() {
    std::vector<obj*> s;
    s.push_back(new concA(3));
    s.push_back(new decA<concA>(10,666));
    s.push_back(new decB<decA<concB>>(2,1,7));
    s.push_back(new decB<concB>(2,6));
    s.push_back(new decB<concB>(2,6));
    for(obj* p:s)std::cout<<p->f()<<std::endl;
}
*/

int main() {
    //for(int y=0;y<w.size_y();y++)for(int x=0;x<w.size_x();x++)w.put_pixel(y,x,x^y);
    rtx::scene s(rtx::color_rgb::from_rgb(0x555555),rtx::color_rgb::from_rgb(0x555555));
    
    rtx::screen w(800,600);
    
    s.add((rtx::scene_object*)new rtx::solid_color<rtx::sphere>(rtx::color_rgb::from_rgb(0x5500ff),rtx::vector3(0.55,-0.16, 3.5),0.5));
    //s.add(new rtx::scene_object());
    rtx::camera c(rtx::vector3(0),rtx::vector3(0,0,1),rtx::vector3(0,1,0),800,600);
    c.render_screen(w,s);
    w.wait_key("Q");
}

/*
class A {
public:
    virtual int f () const {return 0;}
};

class B : public A {
public:
    virtual int f () const override {return 4;}
};

int main() {
    A* a = new B;
    std::cout<<a->f()<<std::endl;
}
*/
