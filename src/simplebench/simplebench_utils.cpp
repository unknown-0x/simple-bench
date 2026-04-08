#include "simplebench_utils.hpp"

#include <cassert>

namespace simplebench {
namespace internal {
void const volatile* volatile global_force_escape_pointer;
void UseCharPointer(char const volatile* const v) {
  global_force_escape_pointer = reinterpret_cast<void const volatile*>(v);
}
}  // namespace internal

const char* TimeUnitToString(const TimeUnit unit) {
  switch (unit) {
    case TimeUnit::kNanosecond:
      return "ns";
    case TimeUnit::kMicrosecond:
      return "us";
    case TimeUnit::kMillisecond:
      return "ms";
    case TimeUnit::kSecond:
      return "s";
  }
  assert(false);
}
}  // namespace simplebench