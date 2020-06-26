#include <rtx/rtx.h>
#include <rtx/detail.h>
#include <thread>

using namespace rtx;
using namespace std::chrono_literals;

void test_vector_1() {
  constexpr vector3 v(0.3, 0.3, 0.3);
}

void test_color() {
  screen s(200, 200, 0xff7f00_rgb);
  s.flush();
  std::this_thread::sleep_for(1s);
}
#include <boost/mpl/vector.hpp>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/io/jpeg.hpp>

template <class T>
constexpr
std::string_view
type_name()
{
  using namespace std;
#ifdef __clang__
  string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
  string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
  return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
  return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
  string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

void test_image() {
  //typedef boost::mpl::vector<boost::gil::gray8_image_t , boost::gil::gray16_image_t,boost::gil::rgb8_image_t , boost::gil::rgb16_image_t> my_img_types;
  //boost::gil::any_image<my_img_types> runtime_image;
  //jpeg_read_image("input.jpg", runtime_image);
  boost::gil::rgb8_image_t img;

  read_image("/home/luke/CLionProjects/rtx/logo3.jpg", img, boost::gil::jpeg_tag());
  std::cerr << img.width() << "x" << img.height() << std::endl;
  screen s(img.width(), img.height());
  std::cerr << sizeof(img) << std::endl;
  std::cerr << sizeof(img._view) << std::endl;
  img._view;
  std::cout << "view has type: " << type_name<decltype(img._view)>() << std::endl;
  typedef boost::gil::image_view<boost::gil::memory_based_2d_locator<boost::gil::memory_based_step_iterator<boost::gil::pixel<unsigned char, boost::gil::layout<boost::mpl::vector3<boost::gil::red_t, boost::gil::green_t, boost::gil::blue_t> > >*> > > gg;
  for (int x = 0; x < img.width(); ++x) {
    for (int y = 0; y < img.height(); ++y) {
      boost::gil::rgb8_pixel_t p = *img._view.at(x, y);
      s.put_pixel(x,y,{uint32_t(boost::gil::at_c<0>(p)),uint32_t(boost::gil::at_c<1>(p)),uint32_t(boost::gil::at_c<2>(p))});
    }
  }

  //boost::gil::rgb8_pixel_t p = *img._view.at(40, 20);
  //img._view.operator[](0).operator[](0).
  //img._view.at(0,0);
  //std::cerr << int(boost::gil::at_c<0>(p)) <<" " <<int(boost::gil::at_c<1>(p)) <<" " <<int(boost::gil::at_c<2>(p)) <<" " << std::endl;
  s.wait_key("Q");
}

void test_stoppable_future() {
  jfuture<int> st = jasynch([](float x, stop_token stop) {
    for (int i = 0; i < 10; ++i) {
      x *= x;
      if (stop.stopped()) {
        std::cerr << "job stopped" << std::endl;
        return i;
      }
      std::this_thread::sleep_for(200ms);
    }
    std::cerr << "job finished" << std::endl;
    return int(std::floor(std::log2(x)));
  }, 1.01);
  st.terminate();
  //st.wait();
  //std::this_thread::sleep_for(0.5s);
  //st.terminate();
  std::cerr << st.get() << std::endl;
}

int main() {
  test_image();
}

