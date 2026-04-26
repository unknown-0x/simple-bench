#ifndef SIMPLEBENCH_SIMPLEBENCH_HPP_
#define SIMPLEBENCH_SIMPLEBENCH_HPP_

#include "simplebench_benchmark.hpp"
#include "simplebench_console_reporter.hpp"
#include "simplebench_reporter.hpp"
#include "simplebench_utils.hpp"

#include <memory>

namespace simplebench {
namespace internal {
SIMPLEBENCH_API Benchmark* RegisterBenchmark(
    std::unique_ptr<simplebench::Benchmark> benchmark);
}

SIMPLEBENCH_API void Initialize(int argc, char** argv);

SIMPLEBENCH_API void SetReporter(Reporter* new_reporter);

SIMPLEBENCH_API int RunAllBenchmarks();

inline Benchmark* RegisterBenchmark(const char* name,
                                    typename FunctionBenchmark::Function fn) {
  return internal::RegisterBenchmark(
      std::make_unique<FunctionBenchmark>(name, fn));
}

template <typename Lambda>
inline Benchmark* RegisterBenchmark(const char* name, Lambda&& fn) {
  return internal::RegisterBenchmark(std::make_unique<LambdaBenchmark<Lambda>>(
      name, std::forward<Lambda>(fn)));
}
}  // namespace simplebench

#endif