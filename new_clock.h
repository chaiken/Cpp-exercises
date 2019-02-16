#ifndef NEW_CLOCK_H
#define NEW_CLOCK_H

#include <cassert>

#include <chrono>
#include <iostream>

using namespace ::std::chrono;

namespace new_clock {

class NewClock {
public:
  NewClock() {
    const time_point<system_clock, duration<int>> tp_seconds(duration<int>(0));
    system_clock::time_point time_(tp_seconds);
  }
  void operator++(int seconds = 1);
  void operator--(int seconds = 1);
  friend ::std::ostream &operator<<(::std::ostream &out, const NewClock &nc);
  long int get_seconds() const;

private:
  time_point<system_clock, duration<int>> time_;
};

::std::ostream &operator<<(::std::ostream &out, const NewClock &nc);
} // namespace new_clock

#endif
