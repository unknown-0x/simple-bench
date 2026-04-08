#include "simplebench_runner.hpp"
#include "simplebench_reporter.hpp"

#include <cassert>
#include <cstring>

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

namespace simplebench {
BenchmarkRunner::BenchmarkRunner() {
#ifdef _WIN32
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

BenchmarkRunner::~BenchmarkRunner() {
  SetReporter(nullptr);
}

void BenchmarkRunner::SetReporter(Reporter* new_reporter) {
  delete reporter_;
  reporter_ = new_reporter;
}

void BenchmarkRunner::RunAllBenchmarks() {
  reporter_->OnStart();
  for (const auto& benchmark : benchmarks_) {
    reporter_->OnBenchmarkStart(benchmark);
    benchmark.Run(reporter_);
    reporter_->OnBenchmarkEnd(benchmark);
  }
  reporter_->OnExit();
}

Benchmark& BenchmarkRunner::RegisterBenchmark(const char* name,
                                              Benchmark::Function function,
                                              Benchmark::Config config) {
  bool found = false;
  for (auto& benchmark : benchmarks_) {
    if (strcmp(benchmark.GetName(), name) == 0) {
      found = true;
    }
  }

  assert(!found);
  benchmarks_.emplace_back(name, function, config);
  return benchmarks_.back();
}

BenchmarkRunner& BenchmarkRunner::Get() {
  static BenchmarkRunner global_runner{};
  return global_runner;
}
}  // namespace simplebench