#ifndef SIMPLEBENCH_SIMPLEBENCH_CONSOLE_REPORTER_HPP_
#define SIMPLEBENCH_SIMPLEBENCH_CONSOLE_REPORTER_HPP_

#include "simplebench_reporter.hpp"

namespace simplebench {
class ConsoleReporter : public Reporter {
 public:
  ConsoleReporter();
  ~ConsoleReporter() = default;

  virtual void OnStart() override;
  virtual void OnBenchmarkStart(const Benchmark&) override;
  virtual void OnBenchmarkEnd(const Benchmark&) override;
  virtual void OnReportStatistics(const Benchmark&,
                                  const BenchmarkStats&) override;
  virtual void OnExit() override;

 private:
  TextStream stream;
};
}  // namespace simplebench

#endif