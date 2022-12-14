cmake_minimum_required(VERSION 3.21)

if(WIN32)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/windows)
  set(ENV_DEP_LIB octane_gui_windows)
  set(app_icon_windows ${CMAKE_CURRENT_SOURCE_DIR}/assets/appicon.rc)
  set(ENV_DEP_SRC ${app_icon_windows})
elseif(APPLE)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/darwin)
  set(ENV_DEP_LIB octane_gui_darwin)
  set(MACOSX_BUNDLE_ICON_FILE icon.icns)
  set(app_icon_macos ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/icon.icns)
  set_source_files_properties(
    ${app_icon_macos}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
  )
  set(ENV_DEP_SRC MACOSX_BUNDLE ${app_icon_macos})
endif()

qt_add_executable(
  octane
  ${ENV_DEP_SRC}
  assets/assets.qrc
  cpp/main.cpp
  cpp/settings.cpp
  cpp/tmp_dir.cpp
  cpp/api.cpp
  cpp/device_info.cpp
  cpp/message_box.cpp
  cpp/main_window.cpp
  cpp/general_panel.cpp
  cpp/room_config_panel.cpp
  cpp/key_config_panel.cpp
  cpp/server_panel.cpp
  cpp/info_panel.cpp
  cpp/clipboard_data.cpp
  cpp/clipboard_manager.cpp
  cpp/task_tray.cpp
)

target_include_directories(octane PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(octane PRIVATE Qt6::Widgets qhotkey octane_api_client ${ENV_DEP_LIB})
