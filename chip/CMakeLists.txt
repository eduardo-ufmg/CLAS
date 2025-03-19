# GabrielGraphBasedClassifiers/chip

# Common library for chip components
add_library(chip_common STATIC
    chipcid.cpp
)
target_include_directories(chip_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add subdirectories
add_subdirectory(chip-clas)
add_subdirectory(rchip-clas)
