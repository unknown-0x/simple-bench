#include <simplebench/simplebench.hpp>

#include <string>
#include <vector>

template <typename Container>
BENCHMARK_TEMPLATE(ContainerOp) {
  using Value = typename Container::value_type;
  Container container({Value(), Value(), Value(), Value(), Value(), Value()});
  container.push_back(Value());
}
BENCHMARK_TEMPLATE_REGISTER(ContainerOp, std::vector<int>)
    .BenchmarkTimeUnit(simplebench::TimeUnit::kMicrosecond);
BENCHMARK_TEMPLATE_REGISTER(ContainerOp, std::vector<std::string>)
    .BenchmarkTimeUnit(simplebench::TimeUnit::kMicrosecond);