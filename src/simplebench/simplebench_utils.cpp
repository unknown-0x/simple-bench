#include "simplebench_utils.hpp"

#include <cassert>

namespace simplebench {
namespace internal {
void const volatile* volatile global_force_escape_pointer;
void UseCharPointer(char const volatile* const v) {
  global_force_escape_pointer = reinterpret_cast<void const volatile*>(v);
}
}  // namespace internal

static constexpr const char* kTimeUnitStrings[] = {"ns", "us", "ms", "s"};
const char* TimeUnitToString(const TimeUnit unit) {
  return kTimeUnitStrings[static_cast<size_t>(unit)];
}

time_point<Clock> AddDuration(time_point<Clock> tp,
                              TimeUnit unit,
                              double value) {
  using Duration = typename Clock::duration;
  switch (unit) {
    case TimeUnit::kSecond:
      return tp +
             duration_cast<Duration>(duration<double, std::ratio<1>>(value));
    case TimeUnit::kMillisecond:
      return tp + duration_cast<Duration>(duration<double, std::milli>(value));
    case TimeUnit::kMicrosecond:
      return tp + duration_cast<Duration>(duration<double, std::micro>(value));
    case TimeUnit::kNanosecond:
      return tp + duration_cast<Duration>(duration<double, std::nano>(value));
    default:
      assert(false);
      return tp;
  }
}

double ToDurationAsDouble(TimeUnit unit,
                          time_point<Clock> start,
                          time_point<Clock> end) {
  switch (unit) {
    case TimeUnit::kSecond:
      return duration<double, std::ratio<1>>(end - start).count();
    case TimeUnit::kMillisecond:
      return duration<double, std::milli>(end - start).count();
    case TimeUnit::kMicrosecond:
      return duration<double, std::micro>(end - start).count();
    case TimeUnit::kNanosecond:
      return duration<double, std::nano>(end - start).count();
    default:
      assert(false);
      return duration<double>(end - start).count();
  }
}
}  // namespace simplebench