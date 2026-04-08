#include "simplebench_macros.hpp"

#include "simplebench_runner.hpp"

namespace simplebench {
namespace internal {
Benchmark& RegisterBenchmark(const char* name,
                             Benchmark::Function function,
                             Benchmark::Config config) {
  return BenchmarkRunner::Get().RegisterBenchmark(name, function, config);
}
}  // namespace internal
}  // namespace simplebench