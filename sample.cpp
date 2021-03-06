#include <set>
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

void scene_1(rtx::scene& s){
  typedef rtx::reflect<rtx::phong_color<rtx::sphere,rtx::color_rgb,rtx::color_rgb>> refsphere;
  typedef rtx::solid_color<rtx::sphere> solsphere;
  s.add(new refsphere(0.2, rtx::color_rgb::from_rgb(0xff0000) * 0.2, rtx::color_rgb::from_rgb(0xff0000) * 0.08, rtx::color_rgb(0.1), 20.0, rtx::vector3(0.00, 0.16, 4.5), 0.3));
  s.add(new refsphere(0.15, rtx::color_rgb::from_rgb(0xffd300) * 0.4, rtx::color_rgb::from_rgb(0xffd300) * 0.08, rtx::color_rgb(0.1), 20.0, rtx::vector3(0.55, -0.16, 3.5), 0.5));
  s.add(new refsphere(0.15, rtx::color_rgb::from_rgb(0x0000ff) * 0.2, rtx::color_rgb::from_rgb(0x0000ff) * 0.08, rtx::color_rgb(0.1), 20.0, rtx::vector3(2.55, 0.16, 3.5), 0.5));
  s.add(new rtx::reflect<rtx::phong_color<rtx::sphere,rtx::color_rgb,rtx::color_rgb>>(0.9, rtx::color_rgb(0.2), rtx::color_rgb(0.2), rtx::color_rgb(0.5), 10.0, rtx::vector3(-0.55, -0.16, 5.5), 0.5));
  s.add(new rtx::sun_light(rtx::vector3(0.5, -2, 1), rtx::color_rgb(1)));
  s.add(new rtx::sun_light(rtx::vector3(-3, -2, 1), rtx::color_rgb(1)));
  typedef rtx::reflect<rtx::phong_color<rtx::plane,rtx::grid_texture,rtx::grid_texture>> refplane;
  s.add(new refplane(0.5,rtx::grid_texture(1),rtx::grid_texture(1),rtx::color_rgb(0.1),20.0,rtx::vector3{0.0,-1.0,4.0},rtx::vector3{0.0,1.0,-0.05}));
  s.add(new rtx::reflect<rtx::phong_color<rtx::plane,rtx::color_rgb,rtx::color_rgb>>(0.8,rtx::color_rgb(0.2),rtx::color_rgb(0.2),rtx::color_rgb(0.1),20.0,rtx::vector3{0.0,-4.0,0.0},rtx::vector3{0.0,1.0,0}));
  s.add(new rtx::transparent<rtx::phong_color<rtx::disk,rtx::color_rgb,rtx::color_rgb>>(0.9,1.458,6,rtx::color_rgb(1),rtx::color_rgb(0.2), rtx::color_rgb(1), 20.0,rtx::vector3{-2.0,0.5,4.0},rtx::vector3{0.0,0.0,1},1.5));
  s.add(new rtx::transparent<rtx::phong_color<rtx::disk,rtx::color_rgb,rtx::color_rgb>>(0.9,1.458,6,rtx::color_rgb(1),rtx::color_rgb(0.2), rtx::color_rgb(1), 20.0,rtx::vector3{-2.0,0.5,4.2},rtx::vector3{0.0,0.0,-1},1.5));

  //s.add(new rtx::phong_color<rtx::cylinder,rtx::color_rgb,rtx::color_rgb>(rtx::color_rgb::from_rgb(0xff0000) * 0.2, rtx::color_rgb(1) * 0.08, rtx::color_rgb(0.1), 20.0,rtx::vector3{1.0,-0.2,5},rtx::vector3{1.0,2.0,0.0},0.2));

}

void scene_2(rtx::scene& s){
  typedef rtx::reflect<rtx::phong_color<rtx::sphere,rtx::color_rgb,rtx::color_rgb>> refsphere;
  typedef rtx::solid_color<rtx::sphere> solsphere;
   s.add(new rtx::sun_light(rtx::vector3(0.5, -2, 1), rtx::color_rgb(1)));
  s.add(new rtx::sun_light(rtx::vector3(-3, -2, 1), rtx::color_rgb(1)));
  typedef rtx::reflect<rtx::phong_color<rtx::plane,rtx::grid_texture,rtx::grid_texture>> refplane;
  s.add(new refplane(0.6,rtx::grid_texture(0.3),rtx::grid_texture(0.3),rtx::color_rgb(0.1),20.0,rtx::vector3{0.0,-1.0,4.0},rtx::vector3{0.0,1.0,0}));
  //s.add(new rtx::reflect<rtx::phong_color<rtx::bumped<rtx::plane>,rtx::color_rgb,rtx::color_rgb>> (0.6,rtx::color_rgb::from_rgb(0x207d0c)*0.3,rtx::color_rgb::from_rgb(0x207d0c)*0.3,rtx::color_rgb(0.1),20.0,80,0.1,rtx::vector3{0.0,-1.0,4.0},rtx::vector3{0.0,1.0,0}));

  s.add(new rtx::phong_color<rtx::cylinder,rtx::color_rgb,rtx::color_rgb>(rtx::color_rgb(1) * 0.2, rtx::color_rgb(1) * 0.08, rtx::color_rgb(0.1), 20.0,rtx::vector3{0.7,-0.8,5},rtx::vector3{-1.8,2.5,0.0},0.05));
  auto whiskey = rtx::color_rgb::from_rgb(0);rtx::color_rgb::from_rgb(0xe38200);
  auto y = rtx::vector3(0,1,0), o =rtx::vector3(0,-1,5);
  constexpr auto eps = rtx::eps*100;
  constexpr rtx::dtype liquid_height = 2, glass_radius= 1, liquid_radius = glass_radius-eps,glass_height=liquid_height+0.3;
  auto glass_thickness = 0.1;
  auto liquid_base = o+y*(glass_thickness+eps);

  //liquid
  s.add(new rtx::transparent<rtx::phong_color<rtx::cylinder,rtx::color_rgb,rtx::color_rgb>>(0.99,1.0f/1.33,25,whiskey,whiskey, rtx::color_rgb(1), 20.0,liquid_base,y*liquid_height,liquid_radius));
  s.add(new rtx::transparent<rtx::phong_color<rtx::disk,rtx::color_rgb,rtx::color_rgb>>(0.99,1.0f/1.33,25,whiskey,whiskey, rtx::color_rgb(1), 20.0,liquid_base+y*liquid_height,y,liquid_radius));
  //glass
//  s.add(new rtx::transparent<rtx::phong_color<rtx::cylinder,rtx::color_rgb,rtx::color_rgb>>(1,1.458,50,rtx::color_rgb(1),rtx::color_rgb(1), rtx::color_rgb(1), 20.0,o,y*glass_height,glass_radius));
//  s.add(new rtx::transparent<rtx::phong_color<rtx::cylinder,rtx::color_rgb,rtx::color_rgb>>(1,1.0f/1.458,50,rtx::color_rgb(1),rtx::color_rgb(1), rtx::color_rgb(1), 20.0,o,y*glass_height,glass_radius+glass_thickness));
//  s.add(new rtx::transparent<rtx::phong_color<rtx::disk_holed,rtx::color_rgb,rtx::color_rgb>>(1,1.0f/1.458,50,rtx::color_rgb(1),rtx::color_rgb(1), rtx::color_rgb(1), 20.0,o,y*glass_height,glass_radius+glass_thickness,glass_radius));

}

int main_2() {

  XInitThreads();


  //for(int y=0;y<w.size_y();y++)for(int x=0;x<w.size_x();x++)w.put_pixel(y,x,x^y);
  rtx::scene s(rtx::color_rgb(0.1), rtx::color_rgb(0.01));
  constexpr int W = 2560,H=1440;
  rtx::screen w(W, H);
  scene_1(s);
  rtx::camera c(rtx::vector3(0,0,0), rtx::vector3(0, 0, 1), rtx::vector3(0, 1, 0), W, H, 45.0, 4);

  bool wire=false;
  size_t rays = 1000000;
  rtx::screen control_screen(200,200);
  while(true){
    auto [stop,wait] = c.render_screen_4(w, s,wire);

    const double angle = 0.05;
    std::cerr<<"waiting key..."<<std::endl;
    int inp = 0;
    std::set<int> inputs = {9,24,90,84,83,85,88,80,113,114,25,65,111,116};
    while(inputs.find(inp)==inputs.end()){inp = control_screen.wait_key();std::cerr<<"inp: "<<inp<<std::endl;}

    std::cerr<<"waiting terminate..."<<std::endl;
    stop.set_value();
    std::cerr<<"waiting termination..."<<std::endl;
    wait.wait();
    std::cerr<<wait.get()<<" rays traced"<<std::endl;

    constexpr double move_amt = 0.05;
    if(inp==24 || inp==9)break;
    else if(inp==90)c.position -= c.depth*move_amt;
    else if(inp==84)c.position += c.depth*move_amt;
    else if(inp==83)c.position -= c.depth.cross(c.vertical)*move_amt;
    else if(inp==85)c.position += c.depth.cross(c.vertical)*move_amt;
    else if(inp==88)c.position -= c.vertical*move_amt;
    else if(inp==80)c.position += c.vertical*move_amt;
//    else if(inp==113 || inp==114){
//      double p = angle;
//      if(inp==114)p*=-1;
//      double cos_p=std::cos(p),sin_p = std::sin(p);
//      double z = c.depth.z, x = c.depth.x;
//      c.depth.z = cos_p*z- sin_p*x;
//      c.depth.x = sin_p*z+cos_p*x;
//      c.depth.normalise();
//    }
    else if(inp==113)c.depth.rotate({0,1,0},-angle),c.vertical.rotate({0,1,0},-angle);
    else if(inp==114)c.depth.rotate({0,1,0},angle),c.vertical.rotate({0,1,0},angle);


    else if(inp==111){
      rtx::vector3 p = c.depth.cross(c.vertical);
      c.depth.rotate(p,-angle);
      c.vertical.rotate(p,-angle);
    }else if(inp==116){
      rtx::vector3 p = c.depth.cross(c.vertical);
      c.depth.rotate(p,angle);
      c.vertical.rotate(p,angle);
    }
    else if(inp==25)wire^=1;
    else if(inp==65){
      std::cerr << "STOP"<<std::endl;
      control_screen.wait_key("R");
    }
    //else if(inp==20){if(rays>2)rays>>=1;}
    //else if(inp==21){if(rays<100000)rays<<=1;}
  }
  return 0;
}

int main(){
  main_2();
  return 0;
}

template<typename R>
bool is_ready(std::future<R> const& f)
{ return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

std::tuple<std::promise<void>,std::future<void>> dumb_fun(){
  std::promise<void> p;
  std::future<void> f = std::async([f = p.get_future()](){
    for(int i=0;i<100000;++i){
      if(is_ready(f)){
        std::cerr<<"early exit!"<<std::endl;
        return;
      }

      std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
    std::cerr<<"normal exit!"<<std::endl;
  });
  return {std::move(p),std::move(f)};
}

std::tuple<std::promise<void>,std::future<void>> dumb_fun2(){
  std::promise<void> p;
  std::future<void> f = std::async([f = p.get_future()](){
    for(int i=0;i<100000;++i){
      if(is_ready(f)){
        std::cerr<<"early exit!"<<std::endl;
        return;
      }

      std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
    std::cerr<<"normal exit!"<<std::endl;
  });
  return {std::move(p),std::move(f)};
}

int main_3() {
  auto [stop,wait] = dumb_fun();
  //terminate.set_value();
  wait.wait();
  //main_2();
  return 0;
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
