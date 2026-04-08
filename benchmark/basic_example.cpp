#include <simplebench/simplebench.hpp>

#include <cmath>

const double pi = std::acos(-1);

BENCHMARK(Sin_v1) {
  const auto s1 = std::sin(pi / 6);
  const auto s2 = std::sin(pi / 2);
  const auto s3 = std::sin(-3 * pi / 4);
}

BENCHMARK(Sin_v2) {
  const auto s1 = std::sin(pi / 6);
  const auto s2 = std::sin(pi / 2);
  const auto s3 = std::sin(-3 * pi / 4);
}

BENCHMARK(Sin_v3) {
  const auto s1 = std::sin(pi / 6);
  const auto s2 = std::sin(pi / 2);
  const auto s3 = std::sin(-3 * pi / 4);
}
