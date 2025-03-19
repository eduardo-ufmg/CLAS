# GabrielGraphBasedClassifiers

cmake_minimum_required(VERSION 3.10)
project(gabrielgraphclassifiers)

# Set build options
option(BUILD_ALL "Build all targets" ON)
option(BUILD_CHIP_LABEL "Build chip-label" ${BUILD_ALL})
option(BUILD_CHIP_TRAIN "Build chip-train" ${BUILD_ALL})
option(BUILD_RCHIP_LABEL "Build rchip-label" ${BUILD_ALL})
option(BUILD_RCHIP_TRAIN "Build rchip-train" ${BUILD_ALL})
option(BUILD_NN_LABEL "Build nn-label" ${BUILD_ALL})
option(BUILD_NN_TRAIN "Build nn-train" ${BUILD_ALL})

# Set output directories
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Prevent in-source builds
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are forbidden. Use a separate build directory.")
endif()


# Add common headers to include path
include_directories(${CMAKE_SOURCE_DIR}/common)

# Add subdirectories
add_subdirectory(common)
add_subdirectory(chip)
add_subdirectory(nn)

# Add common build directory to linker path
link_directories(${CMAKE_SOURCE_DIR}/lib)
