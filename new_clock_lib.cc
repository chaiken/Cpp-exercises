#include "new_clock.h"

#include <ctime>

using namespace std;
using namespace std::chrono;

namespace new_clock {

// Apparently system_clock has a nS tick.  Clearly setting this constant is not
// portable; is there a sycall to read the value rather than a C++ accessor?
constexpr int32_t offset_period = 1000000000;

long int NewClock::get_seconds() const {
  system_clock::duration epoch_offset = time_.time_since_epoch();
  return epoch_offset.count() / offset_period;
}

void NewClock::operator++(int seconds = 1) {
  const duration<int> interval(seconds);
  time_ = time_ + interval;
}

void NewClock::operator--(int seconds) {
  const duration<int> interval(seconds);
  time_ = time_ - interval;
}

ostream &operator<<(ostream &out, const NewClock &nc) {
  time_t this_time_t = system_clock::to_time_t(nc.time_);
  out << ctime(&this_time_t);
  return out;
}

} // namespace new_clock
