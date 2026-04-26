add_executable(
    basic_example
    basic_example.cpp
)
target_link_libraries(basic_example PRIVATE simplebench::simplebench)
target_enable_clang_format(basic_example)
target_enable_clang_tidy(basic_example)

add_executable(
    benchmark_lambda
    benchmark_lambda.cpp
)
target_link_libraries(benchmark_lambda PRIVATE simplebench::simplebench)
target_enable_clang_format(benchmark_lambda)
target_enable_clang_tidy(benchmark_lambda)

add_executable(
    benchmark_with_cfg
    benchmark_with_cfg.cpp
)
target_link_libraries(benchmark_with_cfg PRIVATE simplebench::simplebench)
target_enable_clang_format(benchmark_with_cfg)
target_enable_clang_tidy(benchmark_with_cfg)