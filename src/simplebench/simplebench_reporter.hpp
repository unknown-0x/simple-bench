#ifndef SIMPLEBENCH_SIMPLEBENCH_REPORTER_HPP_
#define SIMPLEBENCH_SIMPLEBENCH_REPORTER_HPP_

#include "simplebench_benchmark.hpp"

namespace simplebench {
class SIMPLEBENCH_API Reporter {
 public:
  virtual ~Reporter() = default;

  virtual void OnStart() {}
  virtual void OnBenchmarkStart(const Benchmark&) = 0;
  virtual void OnBenchmarkEnd(const Benchmark&) = 0;
  virtual void OnReportStatistics(const Benchmark&, const BenchmarkStats&) = 0;
  virtual void OnExit() {}
};
}  // namespace simplebench

#endif