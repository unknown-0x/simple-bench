#ifndef SIMPLEBENCH_SIMPLEBENCH_BENCHMARK_HPP_
#define SIMPLEBENCH_SIMPLEBENCH_BENCHMARK_HPP_

#include "simplebench_utils.hpp"

#include <cassert>
#include <vector>

namespace simplebench {
struct BenchmarkStats {
  size_t total_iterations;
  double total_time;
  double ops_per_unit;
  double time_per_op;
  double median;
  double mean;
  double rsd;
  double std_dev;
  TimeUnit time_unit;
};

struct BenchmarkConfig {
  double MinWarmupTime() const { return warmup_time; }
  BenchmarkConfig& MinWarmupTime(double value) {
    warmup_time = value;
    return *this;
  }

  TimeUnit WarmupTimeUnit() const { return warmup_time_unit; }
  BenchmarkConfig& WarmupTimeUnit(TimeUnit unit) {
    warmup_time_unit = unit;
    return *this;
  }

  bool ShouldWarmup() const { return should_warmup; }
  BenchmarkConfig& ShouldWarmup(bool value) {
    should_warmup = value;
    return *this;
  }

  uint32_t Repeats() const { return repeats; }
  BenchmarkConfig& Repeats(uint32_t value) {
    repeats = value;
    return *this;
  }

  size_t Iterations() const { return iterations; }
  BenchmarkConfig& Iterations(size_t value) {
    iterations = value;
    return *this;
  }

  TimeUnit BenchmarkTimeUnit() const { return benchmark_time_unit; }
  BenchmarkConfig& BenchmarkTimeUnit(TimeUnit unit) {
    benchmark_time_unit = unit;
    return *this;
  }

 private:
  friend class Benchmark;
  // Warmup
  double warmup_time = 500;
  TimeUnit warmup_time_unit = TimeUnit::kNanosecond;
  bool should_warmup = true;

  // Measurement
  uint32_t repeats = 5;
  size_t iterations = 0;
  TimeUnit benchmark_time_unit = TimeUnit::kNanosecond;
};

class Reporter;
class Benchmark {
 public:
  Benchmark(const char* name) : config_(), name_(name) {}
  virtual ~Benchmark() = default;

  virtual void Run(Reporter* reporter) const = 0;

  const std::string& Name() const { return name_; }

  BenchmarkConfig& Config() { return config_; }
  const BenchmarkConfig& Config() const { return config_; }

 private:
  BenchmarkConfig config_;
  std::string name_;
};

namespace internal {
SIMPLEBENCH_API BenchmarkStats
ComputeStats(const std::vector<std::pair<double, size_t>>& samples,
             TimeUnit unit);

SIMPLEBENCH_API void Report_OnBenchmarkStart(Reporter* reporter,
                                             const Benchmark& benchmark);

SIMPLEBENCH_API void Report_OnReportStatistics(Reporter* reporter,
                                               const Benchmark& benchmark,
                                               const BenchmarkStats& stats);

SIMPLEBENCH_API void Report_OnBenchmarkEnd(Reporter* reporter,
                                           const Benchmark& benchmark);

template <typename Fn>
void Warmup(const simplebench::Benchmark& benchmark, Fn&& fn) {
  const BenchmarkConfig& config = benchmark.Config();

  if (!config.ShouldWarmup()) {
    return;
  }

  static constexpr size_t kMaxBatch = 100'000'000;
  const auto warmup_start = Clock::now();
  const auto warmup_end = AddDuration(warmup_start, config.WarmupTimeUnit(),
                                      config.MinWarmupTime());

  size_t batch = 1;
  auto current_time = warmup_start;
  while (current_time < warmup_end) {
    const auto start = current_time;
    for (size_t i = 0; i < batch; ++i) {
      fn();
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

template <typename Fn>
void RunBenchmark(Reporter* reporter,
                  const simplebench::Benchmark& benchmark,
                  Fn&& fn) {
  const BenchmarkConfig& config = benchmark.Config();

  Report_OnBenchmarkStart(reporter, benchmark);
  const TimeUnit benchmark_time_unit = config.BenchmarkTimeUnit();
  const uint32_t repeats = config.Repeats();
  size_t iterations = config.Iterations();
  Warmup(benchmark, std::forward<Fn>(fn));

  if (iterations == 0) {
    static constexpr auto kDiscoveryPeriod = milliseconds(100);
    static constexpr auto kTargetMeasurementTimeNs = 500'000'000.0;
    auto start = Clock::now();
    size_t batch = 1;
    size_t total_executed = 0;
    while (Clock::now() - start < kDiscoveryPeriod) {
      for (size_t i = 0; i < batch; ++i) {
        fn();
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
      fn();
    }
    double duration =
        ToDurationAsDouble(benchmark_time_unit, b_start, Clock::now());
    samples.emplace_back(duration, iterations);
  }

  BenchmarkStats stats = ComputeStats(samples, benchmark_time_unit);
  Report_OnReportStatistics(reporter, benchmark, stats);
  Report_OnBenchmarkEnd(reporter, benchmark);
}
}  // namespace internal

class FunctionBenchmark : public Benchmark {
 public:
  using Function = void (*)();
  FunctionBenchmark(const char* name, Function f) : Benchmark(name), func_(f) {}

  void Run(Reporter* reporter) const override {
    internal::RunBenchmark(reporter, *this, func_);
  }

 private:
  Function func_;
};

template <typename Lambda>
class LambdaBenchmark : public Benchmark {
 public:
  template <typename OtherLambda>
  LambdaBenchmark(const char* name, OtherLambda&& f)
      : Benchmark(name), lambda_(std::forward<OtherLambda>(f)) {}

  void Run(Reporter* reporter) const override {
    internal::RunBenchmark(reporter, *this, lambda_);
  }

 private:
  Lambda lambda_;
};

SIMPLEBENCH_API size_t GetBenchmarkCount();
}  // namespace simplebench

#endif