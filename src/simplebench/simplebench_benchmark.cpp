#include "simplebench_benchmark.hpp"
#include "simplebench_reporter.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <vector>

namespace simplebench {
using namespace std::chrono;
using Clock = steady_clock;

time_point<Clock> AddDuration(time_point<Clock> tp,
                              TimeUnit unit,
                              double value) {
  using Duration = typename Clock::duration;
  switch (unit) {
    case TimeUnit::kSecond:
      return tp +
             duration_cast<Duration>(duration<double, std::ratio<1>>(value));
    case TimeUnit::kMillisecond:
      return tp + duration_cast<Duration>(duration<double, std::milli>(value));
    case TimeUnit::kMicrosecond:
      return tp + duration_cast<Duration>(duration<double, std::micro>(value));
    case TimeUnit::kNanosecond:
      return tp + duration_cast<Duration>(duration<double, std::nano>(value));
    default:
      assert(false);
      return tp;
  }
}

double ToDurationAsDouble(TimeUnit unit,
                          time_point<Clock> start,
                          time_point<Clock> end) {
  switch (unit) {
    case TimeUnit::kSecond:
      return duration<double, std::ratio<1>>(end - start).count();
    case TimeUnit::kMillisecond:
      return duration<double, std::milli>(end - start).count();
    case TimeUnit::kMicrosecond:
      return duration<double, std::micro>(end - start).count();
    case TimeUnit::kNanosecond:
      return duration<double, std::nano>(end - start).count();
    default:
      assert(false);
      return duration<double>(end - start).count();
  }
}

template <class InputIt>
constexpr double Accumulate(InputIt first, InputIt last, double init) {
  for (; first != last; ++first) {
    init += *first;
  }
  return init;
}

Benchmark::Stats ComputeStats(std::vector<std::pair<double, size_t>> samples,
                              TimeUnit unit) {
  Benchmark::Stats s{};
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

Benchmark::Benchmark(const char* name, Function func, const Config& config)
    : config_(config), name_(name), func_(func) {}

void Benchmark::Run(Reporter* reporter) const {
  uint32_t repeats = config_.repeats;
  size_t iterations = config_.iterations;
  Warmup();

  if (iterations == 0) {
    static constexpr auto kDiscoveryPeriod = milliseconds(100);
    static constexpr auto kTargetMeasurementTimeNs = 500'000'000.0;
    auto start = Clock::now();
    size_t batch = 1;
    size_t total_executed = 0;
    while (Clock::now() - start < kDiscoveryPeriod) {
      for (size_t i = 0; i < batch; ++i) {
        func_();
      }
      total_executed += batch;
      batch *= 2;
    }
    double ns_per_iter =
        static_cast<double>(
            duration_cast<nanoseconds>(Clock::now() - start).count()) /
        static_cast<double>(total_executed);
    iterations = static_cast<size_t>(kTargetMeasurementTimeNs / ns_per_iter);
    if (iterations < 1) {
      iterations = 1;
    }
  }

  std::vector<std::pair<double, size_t>> samples;
  samples.reserve(repeats);
  for (uint32_t r = 0; r < repeats; ++r) {
    auto b_start = Clock::now();
    for (size_t i = 0; i < iterations; ++i) {
      func_();
    }
    double duration =
        ToDurationAsDouble(config_.benchmark_time_unit, b_start, Clock::now());
    samples.emplace_back(duration, iterations);
  }

  Stats stats = ComputeStats(samples, config_.benchmark_time_unit);
  reporter->OnReportStatistics(*this, stats);
}

void Benchmark::Warmup() const {
  if (!config_.should_warmup) {
    return;
  }

  static constexpr size_t kMaxBatch = 100'000'000;
  const auto warmup_start = Clock::now();
  const auto warmup_end =
      AddDuration(warmup_start, config_.warmup_time_unit, config_.warmup_time);

  size_t batch = 1;
  auto current_time = warmup_start;
  while (current_time < warmup_end) {
    const auto start = current_time;
    for (size_t i = 0; i < batch; ++i) {
      func_();
    }
    current_time = Clock::now();
    const auto elapsed = current_time - start;

    if (elapsed < milliseconds(10)) {
      batch *= 10;
    } else {
      const auto next_batch =
          static_cast<size_t>(static_cast<double>(batch) * 1.5);
      batch = (next_batch > batch) ? next_batch : batch + 1;
    }
    if (batch > kMaxBatch) {
      batch = kMaxBatch;
    }
  }
}
}  // namespace simplebench