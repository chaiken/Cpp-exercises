#ifndef NEW_CLOCK_H
#define NEW_CLOCK_H

#include <cassert>

#include <chrono>
#include <iostream>

using namespace ::std::chrono;

namespace new_clock {

class NewClock {
public:
  NewClock(int start = 0) {
    const duration<int> dur(start);
    time_ = new time_point<system_clock, duration<int>>(dur);
    assert(nullptr != time_);
  }
  ~NewClock() { delete time_; }
  void operator++(int seconds);
  void operator--(int seconds);
  friend ::std::ostream &operator<<(::std::ostream &out, const NewClock &nc);
  long int GetSeconds() const;

private:
  time_point<system_clock, duration<int>> *time_;
};

::std::ostream &operator<<(::std::ostream &out, const NewClock &nc);
} // namespace new_clock

#endif
