cmake_minimum_required(VERSION 3.22)
project(strgpu LANGUAGES C)

add_executable(benchmarks benchmark/benchmark.c)
target_include_directories(benchmarks PRIVATE include)
#target_link_libraries(benchmarks OpenCL m)
target_link_libraries(benchmarks "-framework OpenCL")