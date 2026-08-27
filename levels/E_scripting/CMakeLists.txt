cmake_minimum_required(VERSION 3.16)

# Per-E checkboxes are declared in levels/CMakeLists.txt (same cmake-gui
# page as L0–L14). This file only dispatches into E*_*/ when enabled.

function(mini_bt_add_e_subdirectory level_opt dir)
  if(NOT ${level_opt})
    return()
  endif()
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt")
    add_subdirectory("${dir}")
  else()
    message(STATUS "mini_bt: ${dir} is ON but has no CMakeLists.txt yet")
  endif()
endfunction()

mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E0 E0_language)
mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E1 E1_environment)
mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E2 E2_tokenizer)
mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E3 E3_ast)
mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E4 E4_pratt_parser)
mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E5 E5_assignment)
mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E6 E6_script_function)
mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E7 E7_tick_hooks)
mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E8 E8_config_integration)
mini_bt_add_e_subdirectory(MINI_BT_LEVEL_E9 E9_hardening)
