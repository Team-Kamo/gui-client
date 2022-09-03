cmake_minimum_required(VERSION 3.21)

add_executable(
  octane
  cpp/main.cpp
  cpp/settings.cpp
  cpp/api.cpp
  cpp/main_window.cpp
  cpp/general_panel.cpp
  cpp/room_config_panel.cpp
  cpp/key_config_panel.cpp
  cpp/info_panel.cpp
  cpp/clipboard_manager.cpp
)

target_include_directories(octane PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/windows)
  set(ENV_DEP_LIB octane_gui_windows)
endif()

target_link_libraries(octane PRIVATE Qt6::Widgets qhotkey octane_api_client ${ENV_DEP_LIB})
