cmake_minimum_required(VERSION 3.21)

add_library(
  octane_gui_darwin
  cpp/osx_clipboard_manager.cpp
  cpp/osx_device_info.cpp
)

target_include_directories(octane_gui_darwin INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(octane_gui_darwin PRIVATE Qt6::Core Qt6::Widgets)
