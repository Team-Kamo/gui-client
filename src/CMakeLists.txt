cmake_minimum_required(VERSION 3.21)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(
  octane-common-library
  main.cpp
)

target_link_libraries(octane-common-library liboctane)
