# GabrielGraphBasedClassifiers/chip/rchip-clas

# RCHIP-label executable
if(BUILD_RCHIP_LABEL)
  add_executable(rchip-label
    label/label.cpp
    label/rchip.cpp
  )
  target_link_libraries(rchip-label common chip_common)
  target_include_directories(rchip-label PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

# RCHIP-train executable
if(BUILD_RCHIP_TRAIN)
  add_executable(rchip-train
    train/train.cpp
    train/computeHyperplanes.cpp
  )
  target_link_libraries(rchip-train common chip_common)
  target_include_directories(rchip-train PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()
