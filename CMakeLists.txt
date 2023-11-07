cmake_minimum_required(VERSION 3.5)
project(OrderOptimizer)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++fs")

# set(SOURCES
# sources/Configuration.h
# sources/Configuration.cpp
# sources/Product.h
# sources/Order.h
# sources/Order.cpp
# sources/tools.h
# )
# add_executable(OrderOptimizer main.cpp ${SOURCES})
# find_package(Threads REQUIRED)
# target_link_libraries(OrderOptimizer stdc++fs Threads::Threads)

# add_executable(OrderOptimizerTests tests/Configuration_test.cpp ${SOURCES})
# target_compile_definitions(OrderOptimizerTests PRIVATE TEST_BUILD)


file(GLOB SOURCES "sources/*.cpp")

add_executable(OrderOptimizer main.cpp ${SOURCES})
find_package(Threads REQUIRED)
target_link_libraries(OrderOptimizer stdc++fs Threads::Threads)

add_executable(OrderOptimizerTests tests/Configuration_test.cpp ${SOURCES})
target_link_libraries(OrderOptimizerTests stdc++fs Threads::Threads)
target_compile_definitions(OrderOptimizerTests PRIVATE TEST_BUILD)

target_include_directories(OrderOptimizer PRIVATE sources)
target_include_directories(OrderOptimizerTests PRIVATE sources)