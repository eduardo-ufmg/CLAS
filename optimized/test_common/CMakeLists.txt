cmake_minimum_required(VERSION 3.10)
project(test_common)

# Build the common library from the sibling directory
add_subdirectory(../common ${CMAKE_CURRENT_BINARY_DIR}/common)

# Create the test executable
add_executable(test test.cpp printContainers.cpp)

# Link the common library and propagate its settings (includes, Protobuf, etc.)
target_link_libraries(test PRIVATE common)

# Enforce C++20 standard for the test executable
set_target_properties(test PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)
