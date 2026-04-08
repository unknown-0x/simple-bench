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
    double MinWarmupTime() const { return warmup_time; }
    Config& MinWarmupTime(double value) {
      warmup_time = value;
      return *this;
    }

    TimeUnit WarmupTimeUnit() const { return warmup_time_unit; }
    Config& WarmupTimeUnit(TimeUnit unit) {
      warmup_time_unit = unit;
      return *this;
    }

    bool ShouldWarmup() const { return should_warmup; }
    Config& ShouldWarmup(bool value) {
      should_warmup = value;
      return *this;
    }

    uint32_t Repeats() const { return repeats; }
    Config& Repeats(uint32_t value) {
      repeats = value;
      return *this;
    }

    size_t Iterations() const { return iterations; }
    Config& Iterations(size_t value) {
      iterations = value;
      return *this;
    }

    TimeUnit BenchmarkTimeUnit() const { return benchmark_time_unit; }
    Config& BenchmarkTimeUnit(TimeUnit unit) {
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
  using Function = void (*)();

  Benchmark(const char* name, Function func, const Config& config);
  ~Benchmark() = default;

  void Run(Reporter* reporter) const;
  const char* GetName() const { return name_; }
  Config& GetConfig() { return config_; }

 private:
  void Warmup() const;

  Config config_;
  const char* name_;
  Function func_;
};
}  // namespace simplebench

#endif