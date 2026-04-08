#ifndef SIMPLEBENCH_SIMPLEBENCH_MACROS_HPP_
#define SIMPLEBENCH_SIMPLEBENCH_MACROS_HPP_

#include "simplebench_benchmark.hpp"

namespace simplebench {
namespace internal {
SIMPLEBENCH_API Benchmark& RegisterBenchmark(const char* name,
                                             Benchmark::Function function,
                                             Benchmark::Config config);
}
}  // namespace simplebench

#define BENCHMARK(name, ...)                              \
  void BenchmarkFunc##name();                             \
  static ::simplebench::Benchmark& _Benchmark##name##_ =  \
      ::simplebench::internal::RegisterBenchmark(         \
          #name, BenchmarkFunc##name,                     \
          ::simplebench::Benchmark::Config(__VA_ARGS__)); \
  void BenchmarkFunc##name()

#endif