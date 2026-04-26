add_library(
    simplebench
    simplebench/simplebench.hpp
    simplebench/simplebench.cpp
    simplebench/simplebench_benchmark.hpp
    simplebench/simplebench_benchmark.cpp
    simplebench/simplebench_console_reporter.hpp
    simplebench/simplebench_console_reporter.cpp
    simplebench/simplebench_reporter.hpp
    simplebench/simplebench_utils.hpp
    simplebench/simplebench_utils.cpp
)

target_include_directories(simplebench PUBLIC ${PROJECT_ROOT}/src)

add_library(simplebench::simplebench ALIAS simplebench)

target_enable_clang_format(simplebench)
target_enable_clang_tidy(simplebench)
target_compiler_warnings(simplebench)
target_export_api(simplebench SIMPLEBENCH_API)