function(add_benchmark name)
    add_executable(
        ${name}
        ${name}.cpp
    )
    target_link_libraries(${name} PRIVATE simplebench::simplebench)
    target_enable_clang_format(${name})
    target_enable_clang_tidy(${name})
    target_import_api(${name} simplebench)

    
if(BUILD_SHARED_LIBS AND MSVC)
  set_target_properties(${name}
    PROPERTIES
      WINDOWS_EXPORT_ALL_SYMBOLS ON
  )
endif()
endfunction()

add_benchmark(basic_example)
add_benchmark(benchmark_lambda)
add_benchmark(benchmark_with_cfg)