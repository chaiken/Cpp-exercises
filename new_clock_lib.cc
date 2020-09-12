#include "new_clock.h"

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
  assert(nullptr != time_);
  system_clock::duration epoch_offset = time_->time_since_epoch();
  return epoch_offset.count() / GetClockPeriod(clockid);
}

void NewClock::operator++(int seconds) {
  assert(nullptr != time_);
  const duration<int> interval(seconds);
  *time_ += interval;
}

void NewClock::operator--(int seconds) {
  assert(nullptr != time_);
  const duration<int> interval(seconds);
  *time_ -= interval;
}

ostream &operator<<(ostream &out, const NewClock &nc) {
  time_t this_time_t = system_clock::to_time_t(*(nc.time_));
  out << ctime(&this_time_t);
  return out;
}

} // namespace new_clock
