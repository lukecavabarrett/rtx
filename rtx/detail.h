#ifndef _INCLUDE_RTX_DETAIL_H_
#define _INCLUDE_RTX_DETAIL_H_

#include <thread>
#include <future>

struct stop_token : private std::future<void> {
  explicit stop_token(std::future<void> &&fv) : std::future<void>(std::move(fv)) {}
  [[nodiscard]] bool stopped() const noexcept { return wait_for(std::chrono::seconds(0)) == std::future_status::ready; }
  using std::future<void>::wait;
  using std::future<void>::wait_for;
  using std::future<void>::wait_until;
  using std::future<void>::future;
};

template<typename Res>
struct jfuture : private std::promise<void>, public std::future<Res> {
  typedef std::future<Res> ft;
  typedef std::promise<void> pv;
  jfuture(ft &&ft_, pv &&pv_) : pv(std::move(pv_)), ft(std::move(ft_)) {}
  void terminate() {
    pv::set_value();
  }
  void terminate_at_thread_exit() {
    pv::set_value_at_thread_exit();
  }
  void terminate_and_wait() {
    pv::set_value();
    ft::wait();
  }
};

template<typename _Fn, typename... _Args>
_GLIBCXX_NODISCARD jfuture<std::invoke_result_t<_Fn, _Args..., stop_token>> jasynch(_Fn &&__fn, _Args &&... __args) {
  typedef std::invoke_result_t<_Fn, _Args..., stop_token> return_type;
  std::promise<void> pv;
  return jfuture<return_type>(std::async(std::launch::async, std::forward<_Fn>(__fn), std::forward<_Args>(__args)..., stop_token(pv.get_future())), std::move(pv));
}

#endif //_INCLUDE_RTX_DETAIL_H_
