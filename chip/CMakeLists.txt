# Common sources for chip components
set(CHIP_COMMON_SOURCES
    computeExperts.cpp
)

# Chip-label executable
if(BUILD_CHIP_LABEL)
    add_executable(chip-label
        chip-clas/label.cpp
        ${CHIP_COMMON_SOURCES}
    )
    target_link_libraries(chip-label common)
    target_include_directories(chip-label PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Rchip-label executable
if(BUILD_RCHIP_LABEL)
    add_executable(rchip-label
        rchip-clas/label.cpp
        ${CHIP_COMMON_SOURCES}
    )
    target_link_libraries(rchip-label common)
    target_include_directories(rchip-label PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Chips-train executable
if(BUILD_CHIPS_TRAIN)
    add_executable(chips-train
        train/train.cpp
        ${CHIP_COMMON_SOURCES}
    )
    target_link_libraries(chips-train common)
    target_include_directories(chips-train PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()