# GabrielGraphBasedClassifiers/chip/chip-clas

# CHIP-label executable
if(BUILD_CHIP_LABEL)
  add_executable(chip-label
    label/label.cpp
    label/chip.cpp
  )
  target_link_libraries(chip-label common chip_common)
  target_include_directories(chip-label PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

# CHIP-train executable
if(BUILD_CHIP_TRAIN)
  add_executable(chip-train
    train/train.cpp
    train/computeExperts.cpp
  )
  target_link_libraries(chip-train common chip_common)
  target_include_directories(chip-train PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()
