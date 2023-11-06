cmake_minimum_required(VERSION 3.5)
project(OrderOptimizer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
sources/Configuration.h
sources/Configuration.cpp
sources/Product.h
sources/tools.h
)
add_executable(OrderOptimizer main.cpp ${SOURCES})

add_executable(OrderOptimizerTests tests/Configuration_test.cpp ${SOURCES})
target_compile_definitions(OrderOptimizerTests PRIVATE TEST_BUILD)

