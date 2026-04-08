#include "simplebench_console_reporter.hpp"
#include "simplebench_runner.hpp"

#include <iostream>

namespace simplebench {
ConsoleReporter::ConsoleReporter() {
  stream.Reserve(2048);
}

constexpr const char* kPrimarySeparator =
    "=========================================================================="
    "======";
constexpr const char* kSecondarySeparator =
    "--------------------------------------------------------------------------"
    "------";

void ConsoleReporter::OnStart() {
  const auto benchmark_count = BenchmarkRunner::Get().GetBenchmarks().size();
  std::cout << kPrimarySeparator << '\n';
  if (benchmark_count == 0) {
    std::cout << "No benchmarks found to run." << std::endl;
    return;
  }
  std::cout << "Number of benchmarks: " << benchmark_count << '\n';
  std::cout << kPrimarySeparator << '\n';
}

void ConsoleReporter::OnBenchmarkStart(const Benchmark& benchmark) {
  stream << "Name: " << benchmark.GetName() << '\n';
}
void ConsoleReporter::OnBenchmarkEnd(const Benchmark&) {
  stream << kSecondarySeparator;
  std::cout << stream << '\n';
  stream.Clear();
}
void ConsoleReporter::OnReportStatistics(const Benchmark&,
                                         const Benchmark::Stats& stats) {
  const char* const time_unit_str = TimeUnitToString(stats.time_unit);
  stream << "Total iterations: " << stats.total_iterations << '\n';
  stream << "Total time: " << stats.total_time << ' ' << time_unit_str << '\n';
  stream << "Ops/" << time_unit_str << ": " << stats.ops_per_unit << '\n';
  stream << time_unit_str << "/op: " << stats.time_per_op << '\n';
  stream << "Median: " << stats.median << ' ' << time_unit_str << '\n';
  stream << "Mean: " << stats.mean << ' ' << time_unit_str << '\n';
  stream << "Stddev: " << stats.std_dev << ' ' << time_unit_str << '\n';
  stream << "RSD: " << stats.rsd << '%' << '\n';
}
void ConsoleReporter::OnExit() {
  std::cout << kPrimarySeparator << '\n';
}
}  // namespace simplebench