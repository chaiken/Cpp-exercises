#include "new_clock.hh"

#include <cstring>
#include <ctime>
#include <limits.h>

#include <iostream>

using namespace std;
using namespace std::chrono;

namespace new_clock {

namespace {

pair<long, bool> GetClockPeriod(unsigned short clockid) {
  struct timespec res;
  if (-1 == clock_getres(clockid, &res)) {
    cerr << strerror(errno) << endl;
    return pair<long, bool>{0L, false};
  }
  return pair<long, bool>{res.tv_nsec * 1e9, true};
}

} // namespace

pair<long, bool> NewClock::GetSeconds(unsigned short clockid) const {
  system_clock::duration epoch_offset = time_.get()->time_since_epoch();
  pair<long, bool> res = GetClockPeriod(clockid);
  if (res.second) {
    return pair<long, bool>{epoch_offset.count() / res.first, true};
  } else {
    return pair<long, bool>{0L, false};
  }
}

void NewClock::operator++(int seconds) {
  const duration<int> interval(seconds);
  *(time_.get()) += interval;
}

void NewClock::operator--(int seconds) {
  const duration<int> interval(seconds);
  *(time_.get()) -= interval;
}

ostream &operator<<(ostream &out, const NewClock &nc) {
  time_t this_time_t = system_clock::to_time_t(*(nc.time_.get()));
  out << ctime(&this_time_t);
  return out;
}

} // namespace new_clock
