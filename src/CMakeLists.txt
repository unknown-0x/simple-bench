add_library(
    simplebench
    simplebench/simplebench.hpp
    simplebench/simplebench_benchmark.hpp
    simplebench/simplebench_benchmark.cpp
    simplebench/simplebench_console_reporter.hpp
    simplebench/simplebench_console_reporter.cpp
    simplebench/simplebench_macros.hpp
    simplebench/simplebench_macros.cpp
    simplebench/simplebench_reporter.hpp
    simplebench/simplebench_runner.hpp
    simplebench/simplebench_runner.cpp
    simplebench/simplebench_utils.hpp
    simplebench/simplebench_utils.cpp
)

add_library(simplebench::simplebench ALIAS simplebench)

target_enable_clang_format(simplebench)
target_enable_clang_tidy(simplebench)
target_compiler_warnings(simplebench)
target_export_api(simplebench SIMPLEBENCH_API)

# simpletest_main 
add_library(
    simplebench_main
    simplebench/simplebench_main.cpp
)

add_library(simplebench::main ALIAS simplebench_main)
target_link_libraries(simplebench_main PUBLIC simplebench::simplebench)
target_include_directories(simplebench_main PUBLIC ${PROJECT_ROOT}/src)

target_import_api(simplebench_main simplebench)
target_compiler_warnings(simplebench_main)
target_enable_clang_format(simplebench_main)
target_enable_clang_tidy(simplebench_main)

if(BUILD_SHARED_LIBS AND MSVC)
  set_target_properties(simplebench_main
    PROPERTIES
      WINDOWS_EXPORT_ALL_SYMBOLS ON
  )
endif()