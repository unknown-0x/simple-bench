#ifndef SIMPLEBENCH_SIMPLEBENCH_BENCHMARK_HPP_
#define SIMPLEBENCH_SIMPLEBENCH_BENCHMARK_HPP_

#include "simplebench_utils.hpp"

#include <limits>

namespace simplebench {
class Reporter;
class SIMPLEBENCH_API Benchmark {
 public:
  struct Stats {
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

  struct Config {
    Config& MinWarmupTime(double value) {
      warmup_time = value;
      return *this;
    }

    Config& WarmupTimeUnit(TimeUnit unit) {
      warmup_time_unit = unit;
      return *this;
    }

    Config& ShouldWarmup(bool value) {
      should_warmup = value;
      return *this;
    }

    Config& Repeats(uint32_t value) {
      repeats = value;
      return *this;
    }

    Config& Iterations(size_t value) {
      iterations = value;
      return *this;
    }

    Config& BenchmarkTimeUnit(TimeUnit unit) {
      benchmark_time_unit = unit;
      return *this;
    }

    // Warmup
    double warmup_time = 500;
    TimeUnit warmup_time_unit = TimeUnit::kNanosecond;
    bool should_warmup = true;

    // Measurement
    uint32_t repeats = 5;
    size_t iterations = 0;
    TimeUnit benchmark_time_unit = TimeUnit::kNanosecond;
  };
  using Function = void (*)();

  Benchmark(const char* name, Function func, const Config& config);
  ~Benchmark() = default;

  void Run(Reporter* reporter) const;
  const char* GetName() const { return name_; }

 private:
  void Warmup() const;

  Config config_;
  const char* name_;
  Function func_;
};
}  // namespace simplebench

#endif