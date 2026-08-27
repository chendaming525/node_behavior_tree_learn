cmake_minimum_required(VERSION 3.16)
project(mini_bt_levels LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Per-level checkboxes (cmake-gui / ccmake / VS CMakeSettings)
# Each ON unit builds using ONLY files inside that level folder.
# -----------------------------------------------------------------------------

option(MINI_BT_LEVEL_L0  "Build L0  environment"              ON)
option(MINI_BT_LEVEL_L1  "Build L1  sync leaves"              ON)
option(MINI_BT_LEVEL_L2  "Build L2  sequence"                 ON)
option(MINI_BT_LEVEL_L3  "Build L3  fallback"                 ON)
option(MINI_BT_LEVEL_L4  "Build L4  stateful async"           ON)
option(MINI_BT_LEVEL_L5  "Build L5  blackboard"               ON)
option(MINI_BT_LEVEL_L6  "Build L6  retry decorator"          ON)
option(MINI_BT_LEVEL_L7  "Build L7  halt propagation"         ON)
option(MINI_BT_LEVEL_L8  "Build L8  graduate (hand-written)"  ON)
option(MINI_BT_LEVEL_L9  "Build L9  ports"                    ON)
option(MINI_BT_LEVEL_L10 "Build L10 factory"                  OFF)
option(MINI_BT_LEVEL_L11 "Build L11 xml parse"                ON)
option(MINI_BT_LEVEL_L12 "Build L12 xml tree"                 ON)
option(MINI_BT_LEVEL_L13 "Build L13 subtree"                  ON)
option(MINI_BT_LEVEL_L14 "Build L14 xml graduate"             ON)

# Extension track E (not L15).
# Master ON  -> E0..E9 appear in cmake-gui as normal options.
# Master OFF -> E0..E9 forced OFF and hidden (advanced).
include(CMakeDependentOption)
option(MINI_BT_LEVEL_E_SCRIPTING "Build E scripting track (master)" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E0 "Build E0  language spec (docs)" OFF
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E1 "Build E1  environment" ON
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E2 "Build E2  tokenizer" OFF
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E3 "Build E3  ast" OFF
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E4 "Build E4  pratt parser" OFF
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E5 "Build E5  assignment" OFF
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E6 "Build E6  script function" OFF
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E7 "Build E7  tick hooks" OFF
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E8 "Build E8  config integration" OFF
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)
cmake_dependent_option(MINI_BT_LEVEL_E9 "Build E9  hardening" OFF
  "MINI_BT_LEVEL_E_SCRIPTING" OFF)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/MiniBtLevel.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/utf-8)
endif()

if(NOT TARGET tinyxml2::tinyxml2)
  add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/tinyxml2"
    "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/tinyxml2"
  )
endif()

message(STATUS "=== mini_bt levels (checkbox summary) ===")
message(STATUS "  L0=${MINI_BT_LEVEL_L0} L1=${MINI_BT_LEVEL_L1} L2=${MINI_BT_LEVEL_L2} L3=${MINI_BT_LEVEL_L3}")
message(STATUS "  L4=${MINI_BT_LEVEL_L4} L5=${MINI_BT_LEVEL_L5} L6=${MINI_BT_LEVEL_L6} L7=${MINI_BT_LEVEL_L7}")
message(STATUS "  L8=${MINI_BT_LEVEL_L8} L9=${MINI_BT_LEVEL_L9} L10=${MINI_BT_LEVEL_L10}")
message(STATUS "  L11=${MINI_BT_LEVEL_L11} L12=${MINI_BT_LEVEL_L12} L13=${MINI_BT_LEVEL_L13} L14=${MINI_BT_LEVEL_L14}")
message(STATUS "  E_SCRIPTING=${MINI_BT_LEVEL_E_SCRIPTING}")
message(STATUS "  E0=${MINI_BT_LEVEL_E0} E1=${MINI_BT_LEVEL_E1} E2=${MINI_BT_LEVEL_E2} E3=${MINI_BT_LEVEL_E3}")
message(STATUS "  E4=${MINI_BT_LEVEL_E4} E5=${MINI_BT_LEVEL_E5} E6=${MINI_BT_LEVEL_E6} E7=${MINI_BT_LEVEL_E7}")
message(STATUS "  E8=${MINI_BT_LEVEL_E8} E9=${MINI_BT_LEVEL_E9}")

if(MINI_BT_LEVEL_L0)
  add_subdirectory(L0_environment)
endif()
if(MINI_BT_LEVEL_L1)
  add_subdirectory(L1_sync_leaves)
endif()
if(MINI_BT_LEVEL_L2)
  add_subdirectory(L2_sequence)
endif()
if(MINI_BT_LEVEL_L3)
  add_subdirectory(L3_fallback)
endif()
if(MINI_BT_LEVEL_L4)
  add_subdirectory(L4_stateful_async)
endif()
if(MINI_BT_LEVEL_L5)
  add_subdirectory(L5_blackboard)
endif()
if(MINI_BT_LEVEL_L6)
  add_subdirectory(L6_retry_decorator)
endif()
if(MINI_BT_LEVEL_L7)
  add_subdirectory(L7_halt_propagation)
endif()
if(MINI_BT_LEVEL_L8)
  add_subdirectory(L8_graduate)
endif()
if(MINI_BT_LEVEL_L9)
  add_subdirectory(L9_ports)
endif()
if(MINI_BT_LEVEL_L10)
  add_subdirectory(L10_factory)
endif()
if(MINI_BT_LEVEL_L11)
  add_subdirectory(L11_xml_parse)
endif()
if(MINI_BT_LEVEL_L12)
  add_subdirectory(L12_xml_tree)
endif()
if(MINI_BT_LEVEL_L13)
  add_subdirectory(L13_subtree)
endif()
if(MINI_BT_LEVEL_L14)
  add_subdirectory(L14_xml_graduate)
endif()
if(MINI_BT_LEVEL_E_SCRIPTING)
  add_subdirectory(E_scripting)
endif()
