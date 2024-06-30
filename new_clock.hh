#ifndef NEW_CLOCK_H
#define NEW_CLOCK_H

#include <chrono>
#include <iostream>
#include <memory>

using namespace ::std::chrono;

namespace new_clock {

class NewClock {
public:
  NewClock(int start = 0)
      : time_(
            new time_point<system_clock, duration<int>>(duration<int>(start))) {
  }
  //  ~NewClock() { delete time_; }
  void operator++(int seconds);
  void operator--(int seconds);
  friend ::std::ostream &operator<<(::std::ostream &out, const NewClock &nc);
  std::pair<long, bool> GetSeconds(unsigned short clockid) const;

private:
  std::unique_ptr<time_point<system_clock, duration<int>>> time_;
};

::std::ostream &operator<<(::std::ostream &out, const NewClock &nc);
} // namespace new_clock

#endif
