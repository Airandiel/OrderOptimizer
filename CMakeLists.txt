cmake_minimum_required(VERSION 3.5)
project(OrderOptimizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -O0 -UNDEBUG")

file(GLOB SOURCES "sources/*.cpp")

add_executable(OrderOptimizer main.cpp ${SOURCES})
find_package(Threads REQUIRED)
target_link_libraries(OrderOptimizer stdc++fs Threads::Threads)

add_executable(OrderOptimizerTests tests/tests.cpp ${SOURCES})
target_link_libraries(OrderOptimizerTests stdc++fs Threads::Threads)
target_compile_definitions(OrderOptimizerTests PRIVATE TEST_BUILD)

target_include_directories(OrderOptimizer PRIVATE sources)
target_include_directories(OrderOptimizerTests PRIVATE sources)