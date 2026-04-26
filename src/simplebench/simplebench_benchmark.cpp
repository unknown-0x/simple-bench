#include "simplebench_benchmark.hpp"
#include "simplebench_reporter.hpp"

#include <algorithm>

namespace simplebench {
template <typename InputIt>
constexpr double Accumulate(InputIt first, InputIt last, double d) {
  for (; first != last; ++first) {
    d += *first;
  }
  return d;
}

namespace internal {
BenchmarkStats ComputeStats(
    const std::vector<std::pair<double, size_t>>& samples,
    TimeUnit unit) {
  BenchmarkStats s{};
  s.time_unit = unit;
  s.total_iterations = 0;
  s.total_time = 0.0;

  std::vector<double> time_per_ops_v;
  for (auto [time, iterations] : samples) {
    s.total_iterations += iterations;
    s.total_time += time;
    time_per_ops_v.push_back(time / static_cast<double>(iterations));
  }

  const size_t n = time_per_ops_v.size();
  const double nd = static_cast<double>(n);
  std::sort(time_per_ops_v.begin(), time_per_ops_v.end());
  if (n % 2 == 0) {
    s.median = (time_per_ops_v[n / 2 - 1] + time_per_ops_v[n / 2]) / 2.0;
  } else {
    s.median = time_per_ops_v[n / 2];
  }

  double sum = Accumulate(time_per_ops_v.begin(), time_per_ops_v.end(), 0.0);
  s.mean = sum / nd;

  double sq_sum = 0.0;
  for (double t : time_per_ops_v) {
    sq_sum += (t - s.mean) * (t - s.mean);
  }
  s.std_dev = std::sqrt(sq_sum / nd);
  s.rsd = (s.std_dev / s.mean) * 100.0;
  s.ops_per_unit = static_cast<double>(s.total_iterations) / s.total_time;
  s.time_per_op = s.total_time / static_cast<double>(s.total_iterations);
  return s;
}

void Report_OnBenchmarkStart(Reporter* reporter, const Benchmark& benchmark) {
  reporter->OnBenchmarkStart(benchmark);
}

void Report_OnReportStatistics(Reporter* reporter,
                               const Benchmark& benchmark,
                               const BenchmarkStats& stats) {
  reporter->OnReportStatistics(benchmark, stats);
}

void Report_OnBenchmarkEnd(Reporter* reporter, const Benchmark& benchmark) {
  reporter->OnBenchmarkEnd(benchmark);
}
}  // namespace internal
}  // namespace simplebench