cmake_minimum_required(VERSION 3.21)

add_library(
  liboctane STATIC
  cpp/main_window.cpp
)

target_link_libraries(liboctane Qt6::Core Qt6::Widgets)
