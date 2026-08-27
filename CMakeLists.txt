cmake_minimum_required(VERSION 3.16)
project(mini_bt_workspace LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Workspace switches (cmake-gui / ccmake / VS CMakeSettings)
#
#   full/   — complete integrated project; default OFF (build via cmake -S full)
#   levels/ — per-unit learning projects; default ON
#
# Per-level checkboxes (L0–L14, E0–E9) are in levels/CMakeLists.txt, not here.
# -----------------------------------------------------------------------------

option(MINI_BT_BUILD_FULL   "Build full/ integrated project" OFF)
option(MINI_BT_BUILD_LEVELS "Build levels/ per-unit learning projects" ON)

if(NOT TARGET tinyxml2::tinyxml2)
  add_subdirectory(3rdparty/tinyxml2)
endif()

if(MINI_BT_BUILD_FULL)
  add_subdirectory(full)
endif()

if(MINI_BT_BUILD_LEVELS)
  add_subdirectory(levels)
endif()

if(NOT MINI_BT_BUILD_FULL AND NOT MINI_BT_BUILD_LEVELS)
  message(FATAL_ERROR "Enable at least one of MINI_BT_BUILD_FULL or MINI_BT_BUILD_LEVELS")
endif()
