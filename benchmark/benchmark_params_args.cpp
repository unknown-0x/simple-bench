#include <cmath>
#include <simplebench/simplebench.hpp>

BENCHMARK_PARAMS(CosAndSin, double value) {
  const auto c = std::cos(value);
  const auto s = std::sin(value);
}
const double pi = std::acos(-1);
BENCHMARK_ARGS(CosAndSin, HalfPi, pi / 2.0);
BENCHMARK_ARGS(CosAndSin, QuarterPi, pi / 4.0);
///////////////////////////////////////////////////////////////////
BENCHMARK_PARAMS(StringOp, const char* s) {
  std::string str = s;
  str.append(s);
}
BENCHMARK_ARGS(StringOp, HelloWorld, "hello world")
    .BenchmarkTimeUnit(simplebench::TimeUnit::kMicrosecond);
BENCHMARK_ARGS(StringOp, FooBar, "foo bar")
    .BenchmarkTimeUnit(simplebench::TimeUnit::kMicrosecond);