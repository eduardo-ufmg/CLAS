cmake_minimum_required(VERSION 3.10)
project(nn)

# Build the common library from the sibling directory
add_subdirectory(../../common ${CMAKE_CURRENT_BINARY_DIR}/common)

# List of executables
set(EXECUTABLES train)

# Add an option to enable debug build
option(DEBUG "Enable debug output" OFF)

if(DEBUG)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
endif()

# Set include directory
include_directories(../)

# Create the executable
add_executable(train train.cpp ../computeSVs.cpp ../nearestSVlabel.cpp)

# Link the common library and propagate its settings (includes, Protobuf, etc.)
target_link_libraries(train PRIVATE common)

# Enforce C++20 standard for the executable
set_target_properties(train PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)
