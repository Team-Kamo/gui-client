cmake_minimum_required(VERSION 3.21)

project(octane-common-client CXX)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
endif()

if(WIN32)
  set(CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif(WIN32)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_DEFAULT_MAJOR_VERSION 6)

find_package(Qt6 REQUIRED COMPONENTS Widgets REQUIRED)
qt_standard_project_setup()

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_DIR ${ROOT_DIR}/src)

include_directories(${ROOT_DIR}/QHotkey/QHotkey)
add_subdirectory(${ROOT_DIR}/QHotkey)

# include_directories(${ROOT_DIR}/api-client/src)
# add_subdirectory(${ROOT_DIR}/api-client)
add_subdirectory(${SRC_DIR})
