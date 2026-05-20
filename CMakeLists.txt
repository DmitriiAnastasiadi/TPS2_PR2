cmake_minimum_required(VERSION 3.10)
project(FileAnalyzer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(file_analyzer src/main.cpp src/analyzer.cpp)
target_include_directories(file_analyzer PRIVATE src)

# Test runner
add_executable(test_runner tests/test_analyzer.cpp tests/test_framework.h)
target_include_directories(test_runner PRIVATE src tests)
target_compile_features(test_runner PRIVATE cxx_std_17)

