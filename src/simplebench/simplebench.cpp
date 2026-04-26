#include "simplebench.hpp"

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

#include <vector>

namespace simplebench {
struct SimpleBenchState {
  SimpleBenchState() = default;

  ~SimpleBenchState() { SetReporter(nullptr); }

  void SetReporter(Reporter* new_reporter) {
    delete reporter;
    reporter = new_reporter;
  }

  std::vector<std::unique_ptr<Benchmark>> benchmarks{};
  Reporter* reporter = nullptr;
};

namespace {
static SimpleBenchState simplebench_state{};
}

void Initialize(int argc, char** argv) {
  (void)argc;
  (void)argv;
#ifdef _WIN32
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  (void)_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  (void)_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif
}

void SetReporter(Reporter* new_reporter) {
  simplebench_state.SetReporter(new_reporter);
}

int RunAllBenchmarks() {
  if (!simplebench_state.reporter) {
    simplebench_state.SetReporter(new ConsoleReporter());
  }

  Reporter* reporter = simplebench_state.reporter;
  reporter->OnStart();
  for (const auto& benchmark : simplebench_state.benchmarks) {
    benchmark->Run(reporter);
  }
  reporter->OnExit();
  return 0;
}

size_t GetBenchmarkCount() {
  return simplebench_state.benchmarks.size();
}

namespace internal {
Benchmark* RegisterBenchmark(
    std::unique_ptr<simplebench::Benchmark> benchmark) {
  auto* ptr = benchmark.get();
  simplebench_state.benchmarks.emplace_back(std::move(benchmark));
  return ptr;
}
}  // namespace internal
}  // namespace simplebench