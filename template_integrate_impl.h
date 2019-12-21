#include <algorithm>
#include <functional>
#include <numeric>
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

template <typename T> T do_integrate(const ::std::vector<T> &acont) {
  return (::std::accumulate(acont.begin(), acont.end(), 0.0) / acont.size());
}

} // namespace integration
