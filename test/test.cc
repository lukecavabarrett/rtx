#include <rtx/rtx.h>
#include <rtx/detail.h>
#include <thread>

using namespace rtx;
using namespace std::chrono_literals;

void test_vector_1(){
  constexpr vector3 v(0.3,0.3,0.3);
}

void test_color(){
  screen s(200,200,0xff7f00_rgb);
  s.flush();
  std::this_thread::sleep_for(1s);
}

void test_Xlib(){

}

void test_stoppable_future(){
  jfuture<int> st = jasynch([](float x, stop_token stop) {
    for(int i=0;i<10;++i){
      x*=x;
      if(stop.stopped()){
        std::cerr<<"job stopped"<<std::endl;
        return i;}
      std::this_thread::sleep_for(200ms);
    }
    std::cerr<<"job finished"<<std::endl;
    return int(std::floor(std::log2(x)));
  },1.01);
  st.terminate();
  //st.wait();
  //std::this_thread::sleep_for(0.5s);
  //st.terminate();
  std::cerr << st.get() << std::endl;
}

int main(){
  test_stoppable_future();
  std::this_thread::sleep_for(5s);
}

