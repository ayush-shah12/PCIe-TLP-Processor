cmake_minimum_required(VERSION 3.10)
project(PCIe_TLP_Processor LANGUAGES C CXX)

# Set the C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include test/inputs)

# Define the main executable
add_executable(PCIe-TLP-Processor src/TLP-Processor.c)

# Compiler options for the main executable
target_compile_options(PCIe_TLP_Processor PUBLIC 
    -Wall 
    -Wextra 
    -Wshadow 
    -Wpedantic 
    -Wdouble-promotion 
    -Wformat=2 
    -Wundef 
    -Werror
)

target_link_libraries(PCIe-TLP-Processor PUBLIC m)

# Add test executables if needed
set(TEST_SUITES "parse_tests" "read_unit_test" "write_unit_test")

foreach(TEST_SUITE IN LISTS TEST_SUITES)
    add_executable(${TEST_SUITE} tests/src/${TEST_SUITE}.cpp src/TLP-Processor.c)
    target_compile_options(${TEST_SUITE} PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
    target_include_directories(${TEST_SUITE} PUBLIC include tests/include)
    target_link_libraries(${TEST_SUITE} PUBLIC gtest gtest_main pthread)
endforeach()
