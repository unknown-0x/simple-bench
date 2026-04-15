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
}  // namespace simplebench