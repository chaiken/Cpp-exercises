// $ g++ -std=c++11 -ggdb -Wall -Wextra -g rand_uint16.cc -o rand_uint16
#include <cassert>
#include <cstdlib>
#include <sys/time.h>

#include <iostream>

using namespace std;

// 1597u = 0x63D
constexpr uint16_t kPwmMaxDutyCycle0 = static_cast<uint16_t>(0.39 * 4095);
// 327u = 0x147
constexpr uint16_t kPwmMinDutyCycle0 = static_cast<uint16_t>(0.08 * 4095);

// time_t time(time_t *tloc);
// time() returns the time as the number of seconds since the Epoch, 1970-01-01
// If tloc is non-NULL, the return value is also stored in the memory pointed to
// by tloc.
void SetRandomSeed() { srand(static_cast<unsigned>(time(nullptr))); }

uint16_t GetRandPwmPhaseOffset(const uint16_t max_duty_cycle) {
  SetRandomSeed();
  assert(max_duty_cycle < static_cast<uint16_t>(4095));
  int value = rand() % max_duty_cycle;
  cout << "Integer random value is " << std::hex << "0x" << value << endl;
  return static_cast<uint16_t>(value);
}

std::pair<int, int> GetBytes(uint16_t val) {
  std::pair<int, int> bytes;
  bytes.first = static_cast<int>(val & 0xFF);
  bytes.second = static_cast<int>(val >> 8);
  return bytes;
}

void CheckConsistency(uint16_t val, std::pair<int, int> bytevals) {
  assert(bytevals.first == (val & 0xFF));
  assert(bytevals.second == (val >> 8));
  assert(val == ((bytevals.second << 8) + bytevals.first));
  std::cout << "0x" << val << " is okay" << std::endl;
}

int main() {
  uint16_t on_min = GetRandPwmPhaseOffset(kPwmMinDutyCycle0);
  uint16_t off_min = on_min + kPwmMinDutyCycle0;
  assert(off_min > on_min);
  assert(off_min <= static_cast<uint16_t>(0x1000));
  std::pair<int, int> on_min_bytevals = GetBytes(on_min);
  std::pair<int, int> off_min_bytevals = GetBytes(off_min);
  std::cout << ": on_min_l 0x" << std::hex << on_min_bytevals.first
            << " on_min_h 0x" << std::hex << on_min_bytevals.second << endl;
  std::cout << ": off_min_l 0x" << std::hex << off_min_bytevals.first
            << " off_min_h 0x" << std::hex << off_min_bytevals.second << endl;
  CheckConsistency(on_min, on_min_bytevals);
  CheckConsistency(off_min, off_min_bytevals);
  std::cout << std::endl;

  uint16_t on_max = GetRandPwmPhaseOffset(kPwmMaxDutyCycle0);
  uint16_t off_max = on_max + kPwmMaxDutyCycle0;
  assert(off_max > on_max);
  assert(off_max <= static_cast<uint16_t>(0x1000));
  std::pair<int, int> on_max_bytevals = GetBytes(on_max);
  std::pair<int, int> off_max_bytevals = GetBytes(off_max);
  std::cout << ": on_max_l 0x" << std::hex << on_max_bytevals.first
            << " on_max_h 0x" << std::hex << on_max_bytevals.second << endl;
  std::cout << ": off_max_l 0x" << std::hex << off_max_bytevals.first
            << " off_max_h 0x" << std::hex << off_max_bytevals.second << endl;
  CheckConsistency(on_max, on_max_bytevals);
  CheckConsistency(off_max, off_max_bytevals);

  std::cout << std::endl;
  uint16_t phase = (on_max > on_min) ? (on_max - on_min) : (on_min - on_max);
  std::cout << "Phase difference between the two signals is 0x" << phase
            << "/0x1000." << std::endl;

  exit(0);
}
