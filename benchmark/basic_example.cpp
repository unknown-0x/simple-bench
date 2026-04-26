#include <simplebench/simplebench.hpp>

#include <cmath>

const double pi = std::acos(-1);

void compute_sin() {
  const auto s1 = std::sin(pi / 6.0);
  const auto s2 = std::sin(pi / 2.0);
  const auto s3 = std::sin(-3.0 * pi / 4.0);
}

int main(int argc, char** argv) {
  simplebench::Initialize(argc, argv);

  simplebench::RegisterBenchmark("sin() v1", compute_sin);
  simplebench::RegisterBenchmark("sin() v2", compute_sin);
  simplebench::RegisterBenchmark("sin() v3", compute_sin);

  return simplebench::RunAllBenchmarks();
}