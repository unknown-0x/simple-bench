add_executable(
    benchmark_example
    main.cpp
)

target_link_libraries(benchmark_example simplebench::main)
target_enable_clang_format(benchmark_example)
target_enable_clang_tidy(benchmark_example)