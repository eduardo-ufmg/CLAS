# GabrielGraphBasedClassifiers/chip

# Common sources for chip components
set(CHIP_COMMON_SOURCES
    chipcid.cpp
)

# CHIP-label executable
if(BUILD_CHIP_LABEL)
    add_executable(chip-label
        chip-clas/label/label.cpp
        chip-clas/label/chip.cpp
        ${CHIP_COMMON_SOURCES}
    )
    target_link_libraries(chip-label common)
    target_include_directories(chip-label PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# RCHIP-label executable
if(BUILD_RCHIP_LABEL)
    add_executable(rchip-label
        rchip-clas/label/label.cpp
        rchip-clas/label/rchip.cpp
        ${CHIP_COMMON_SOURCES}
    )
    target_link_libraries(rchip-label common)
    target_include_directories(rchip-label PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# CHIP-train executable
if(BUILD_CHIP_TRAIN)
    add_executable(chips-train
        chip-clas/train/train.cpp
        chip-clas/train/computeExperts.cpp
        ${CHIP_COMMON_SOURCES}
    )
    target_link_libraries(chips-train common)
    target_include_directories(chips-train PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# RCHIP-train executable
if(BUILD_RCHIP_TRAIN)
    add_executable(rchips-train
        rchip-clas/train/train.cpp
        rchip-clas/train/computeExperts.cpp
        ${CHIP_COMMON_SOURCES}
    )
    target_link_libraries(rchips-train common)
    target_include_directories(rchips-train PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()
