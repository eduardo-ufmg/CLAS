cmake_minimum_required(VERSION 3.10)
project(nn)

# Build the common library from the sibling directory
add_subdirectory(../common ${CMAKE_CURRENT_BINARY_DIR}/common)

# List of executables
set(EXECUTABLES label train)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set compiler flags for debug build
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

foreach(EXE IN LISTS EXECUTABLES)
    # Create the executable
    add_executable(${EXE} ${EXE}.cpp nearestSVlabel.cpp)

    # Link the common library and propagate its settings (includes, Protobuf, etc.)
    target_link_libraries(${EXE} PRIVATE common)

    # Enforce C++20 standard for the executable
    set_target_properties(${EXE} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
    )
endforeach()
