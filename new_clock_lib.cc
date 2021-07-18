#include "new_clock.h"

#include <limits.h>

#include <cassert>
#include <ctime>

using namespace std;
using namespace std::chrono;

namespace new_clock {

namespace {

long GetClockPeriod(unsigned short clockid) {
  struct timespec res;
  if (-1 == clock_getres(clockid, &res)) {
    assert_perror(errno);
  }
  return res.tv_nsec * 1e9;
}

} // namespace

long int NewClock::GetSeconds(unsigned short clockid) const {
  assert(nullptr != time_.get());
  system_clock::duration epoch_offset = time_.get()->time_since_epoch();
  const long int period = GetClockPeriod(clockid);
  assert((0LU != period) && (LONG_MAX != period));
  return epoch_offset.count() / period;
}

void NewClock::operator++(int seconds) {
  assert(nullptr != time_.get());
  const duration<int> interval(seconds);
  *(time_.get()) += interval;
}

void NewClock::operator--(int seconds) {
  assert(nullptr != time_.get());
  const duration<int> interval(seconds);
  *(time_.get()) -= interval;
}

ostream &operator<<(ostream &out, const NewClock &nc) {
  time_t this_time_t = system_clock::to_time_t(*(nc.time_.get()));
  out << ctime(&this_time_t);
  return out;
}

} // namespace new_clock
