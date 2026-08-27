cmake_minimum_required(VERSION 3.16)
project(mini_bt_full LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# MSVC: treat source as UTF-8 so Chinese comments in headers do not break parsing
if(MSVC)
  add_compile_options(/utf-8)
endif()

if(NOT TARGET tinyxml2::tinyxml2)
  add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/tinyxml2"
    "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/tinyxml2"
  )
endif()

add_library(mini_bt INTERFACE)
target_include_directories(mini_bt INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(mini_bt INTERFACE tinyxml2::tinyxml2)

add_executable(mini_bt_full demos/main.cpp)
target_link_libraries(mini_bt_full PRIVATE mini_bt)
target_compile_definitions(mini_bt_full PRIVATE
  FULL_ENTER_ROOM_XML="${CMAKE_CURRENT_SOURCE_DIR}/demos/enter_room.xml"
)
