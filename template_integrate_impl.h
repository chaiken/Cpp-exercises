#include <cassert>

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

namespace integration {

template <typename T> class CubeIt {
public:
  CubeIt(T start, T increm) : val_(start), increm_(increm) {}
  T operator()() {
    val_ += increm_;
    return val_ * val_ * val_;
  }

private:
  T val_ = 0.0;
  T increm_ = 0.01;
};

// https://stackoverflow.com/questions/2582161/function-pointer-as-parameter
template <typename T> class FunctionWrapper {
  typedef T(integrated_func(T));

public:
  FunctionWrapper(T start, T increm) : val_(start), increm_(increm) {}
  FunctionWrapper() : val_(0.0), increm_(0.001) {}
  T operator()() {
    assert(nullptr != fn_);
    // ::std::function::operator bool
    // assert(true == fn_);
    val_ += increm_;
    return fn_(val_);
  }
  // Does not work with the constructor?
  void SetFn(integrated_func fn) { fn_ = fn; }

private:
  T val_;
  T increm_;
  //::std::function<T(T)> *fn_;
  // template_integrate_lib_test.cc:48:12: error: cannot convert ‘double
  // (*)(double)’ to ‘std::function<double(double)>*’ T *fn_(T);
  integrated_func *fn_;
};

template <typename T> T do_integrate(const ::std::vector<T> &acont) {
  return (::std::accumulate(acont.begin(), acont.end(), 0.0) / acont.size());
}

// "Using class to introduce a type parameter is equivalent to using typename"
// --Stroustrup, p. 80 http://www.cplusplus.com/reference/algorithm/generate/
template <typename ForwardIterator, typename P>
void call_fill(ForwardIterator first, ForwardIterator last, P funcobj) {
  while (first != last) {
    *first = funcobj();
    ++first;
  }
}

template <typename ForwardIterator, typename T, typename U>
void FillWithLambda(ForwardIterator first, ForwardIterator last, T start,
                    T increm, const U &fill_function) {
  T val = start;
  while (first != last) {
    *first = fill_function(val);
    val += increm;
    ++first;
  }
}

} // namespace integration
