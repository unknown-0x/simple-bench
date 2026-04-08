add_executable(
    basic_example
    basic_example.cpp
)
target_link_libraries(basic_example simplebench::main)
target_enable_clang_format(basic_example)
target_enable_clang_tidy(basic_example)

add_executable(
    benchmark_params_args
    benchmark_params_args.cpp
)
target_link_libraries(benchmark_params_args simplebench::main)
target_enable_clang_format(benchmark_params_args)
target_enable_clang_tidy(benchmark_params_args)

add_executable(
    benchmark_template
    benchmark_template.cpp
)
target_link_libraries(benchmark_template simplebench::main)
target_enable_clang_format(benchmark_template)
target_enable_clang_tidy(benchmark_template)