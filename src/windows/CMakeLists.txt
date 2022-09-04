cmake_minimum_required(VERSION 3.21)

add_library(
  octane_gui_windows
  cpp/win_clipboard_manager.cpp
  cpp/win_device_info.cpp
)

target_include_directories(octane_gui_windows INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(octane_gui_windows PRIVATE Qt6::Core Gdiplus Qt6::Widgets)
