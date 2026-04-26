#include <simplebench/simplebench.hpp>

#include <cmath>

int main(int argc, char** argv) {
  simplebench::Initialize(argc, argv);

  const double pi = std::acos(-1);

  simplebench::RegisterBenchmark("sin() v1", [&] {
    const auto s1 = std::sin(pi / 6.0);
    const auto s2 = std::sin(pi / 2.0);
    const auto s3 = std::sin(-3.0 * pi / 4.0);
  });

  simplebench::RegisterBenchmark("sin() v2", [&] {
    const auto s1 = std::sin(pi / 6.0);
    const auto s2 = std::sin(pi / 2.0);
    const auto s3 = std::sin(-3.0 * pi / 4.0);
  });

  simplebench::RegisterBenchmark("sin() v3", [&] {
    const auto s1 = std::sin(pi / 6.0);
    const auto s2 = std::sin(pi / 2.0);
    const auto s3 = std::sin(-3.0 * pi / 4.0);
  });

  return simplebench::RunAllBenchmarks();
}