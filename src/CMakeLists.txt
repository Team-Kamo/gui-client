cmake_minimum_required(VERSION 3.21)

add_executable(
  octane
  cpp/main.cpp
  cpp/main_window.cpp
  cpp/general_panel.cpp
  cpp/room_config_panel.cpp
  cpp/info_panel.cpp
)

target_include_directories(octane PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(octane PRIVATE Qt6::Widgets qhotkey)