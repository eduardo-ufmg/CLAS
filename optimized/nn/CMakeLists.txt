cmake_minimum_required(VERSION 3.10)
project(nn)

# Build the common library from the sibling directory
add_subdirectory(../common ${CMAKE_CURRENT_BINARY_DIR}/common)

# List of executables
set(EXECUTABLES label train)

foreach(EXE IN LISTS EXECUTABLES)
    # Create the executable
    add_executable(${EXE} ${EXE}.cpp)

    # Link the common library and propagate its settings (includes, Protobuf, etc.)
    target_link_libraries(${EXE} PRIVATE common)

    # Enforce C++20 standard for the executable
    set_target_properties(${EXE} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
    )
endforeach()
