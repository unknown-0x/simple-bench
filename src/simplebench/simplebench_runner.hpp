#ifndef SIMPLEBENCH_SIMPLEBENCH_RUNNER_HPP_
#define SIMPLEBENCH_SIMPLEBENCH_RUNNER_HPP_

#include <vector>
#include "simplebench_benchmark.hpp"

namespace simplebench {
class Reporter;
class SIMPLEBENCH_API BenchmarkRunner {
 public:
  BenchmarkRunner();
  ~BenchmarkRunner();

  const std::vector<Benchmark>& GetBenchmarks() const { return benchmarks_; }
  void SetReporter(Reporter* new_reporter);
  void RunAllBenchmarks();
  Benchmark& RegisterBenchmark(const char* name,
                               Benchmark::Function function,
                               Benchmark::Config config);

  static BenchmarkRunner& Get();

 private:
  std::vector<Benchmark> benchmarks_;
  Reporter* reporter_;
};
}  // namespace simplebench

#endif