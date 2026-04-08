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

#define BENCHMARK_PARAMS(name, ...) void BenchmarkFunc##name(__VA_ARGS__)

#define BENCHMARK_ARGS(name, secondary_name, ...)       \
  static ::simplebench::Benchmark::Config&              \
      _BenchmarkConfig_##name##secondary_name##_ =      \
          ::simplebench::internal::RegisterBenchmark(   \
              #name "/" #secondary_name,                \
              [] { BenchmarkFunc##name(__VA_ARGS__); }, \
              ::simplebench::Benchmark::Config())       \
              .GetConfig()

#define BENCHMARK_TEMPLATE(name) void BenchmarkTemplate##name()

#if defined(__COUNTER__)
#define SIMPLEBENCH_UNIQUE_ID __COUNTER__
#else
#define SIMPLEBENCH_UNIQUE_ID __LINE__
#endif

#define SIMPLEBENCH_CONCAT_IMPL(x, y) x##y
#define SIMPLEBENCH_CONCAT(x, y) SIMPLEBENCH_CONCAT_IMPL(x, y)

#define BENCHMARK_TEMPLATE_REGISTER(name, ...)                 \
  static ::simplebench::Benchmark::Config& SIMPLEBENCH_CONCAT( \
      _BenchmarkConfig_##name, SIMPLEBENCH_UNIQUE_ID) =        \
      ::simplebench::internal::RegisterBenchmark(              \
          #name "<" #__VA_ARGS__ ">",                          \
          [] { BenchmarkTemplate##name<__VA_ARGS__>(); },      \
          ::simplebench::Benchmark::Config())                  \
          .GetConfig()

#endif