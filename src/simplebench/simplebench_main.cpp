#include "simplebench/simplebench.hpp"

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  simplebench::BenchmarkRunner::Get().SetReporter(
      new simplebench::ConsoleReporter());
  simplebench::BenchmarkRunner::Get().RunAllBenchmarks();
  return 0;
}