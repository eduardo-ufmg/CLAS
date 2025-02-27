cmake_minimum_required(VERSION 3.10)
project(CommonLib)

# Find Protobuf
find_package(Protobuf REQUIRED)

# List all the source files (headers need not be compiled, but can be added for IDE organization)
set(COMMON_SOURCES
    classifier.pb.cc
    computeSVs.cpp
    filter.cpp
    gabrielGraph.cpp
    readFiles.cpp
    squaredDistance.cpp
    types.cpp
    writeFiles.cpp
)

# Create a static library target named "common"
add_library(common STATIC
                ${COMMON_SOURCES})

# Specify the include directory (this directory contains the header files)
target_include_directories(common PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}
                            ${Protobuf_INCLUDE_DIRS})

set_target_properties(common PROPERTIES
                        CXX_STANDARD 20
                        CXX_STANDARD_REQUIRED YES
)

target_link_libraries(common PUBLIC
                        ${Protobuf_LIBRARIES})
