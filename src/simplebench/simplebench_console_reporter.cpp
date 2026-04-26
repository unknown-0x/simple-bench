#include "simplebench_console_reporter.hpp"

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
  const auto benchmark_count = GetBenchmarkCount();
  std::cout << kPrimarySeparator << '\n';
  if (benchmark_count == 0) {
    std::cout << "No benchmarks found to run." << std::endl;
    return;
  }
  std::cout << "Number of benchmarks: " << benchmark_count << '\n';
  std::cout << kPrimarySeparator << '\n';
}

void ConsoleReporter::OnBenchmarkStart(const Benchmark& benchmark) {
  stream << "Name: " << benchmark.Name() << '\n';
}

void ConsoleReporter::OnBenchmarkEnd(const Benchmark&) {
  stream << kSecondarySeparator;
  std::cout << stream << '\n';
  stream.Clear();
}

void ConsoleReporter::OnReportStatistics(const Benchmark&,
                                         const BenchmarkStats& stats) {
  stream << "Total iterations: " << stats.total_iterations << '\n';
  stream << "Total time: " << stats.total_time << ' ' << stats.time_unit
         << '\n';
  stream << "Ops/" << stats.time_unit << ": " << stats.ops_per_unit << '\n';
  stream << stats.time_unit << "/op: " << stats.time_per_op << '\n';
  stream << "Median: " << stats.median << ' ' << stats.time_unit << '\n';
  stream << "Mean: " << stats.mean << ' ' << stats.time_unit << '\n';
  stream << "Stddev: " << stats.std_dev << ' ' << stats.time_unit << '\n';
  stream << "RSD: " << stats.rsd << '%' << '\n';
}

void ConsoleReporter::OnExit() {
  std::cout << kPrimarySeparator << '\n';
}
}  // namespace simplebench