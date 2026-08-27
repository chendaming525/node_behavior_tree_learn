# Each level is a self-contained unit: only files in this folder are used.
#
#   mini_bt_add_level_executable(level_l0_hello
#     SOURCES l0_hello.cpp
#   )
#
# All *.hpp in the level directory are exposed as #include "mini_bt/xxx.hpp".

function(mini_bt_add_level_executable target_name)
  cmake_parse_arguments(ARG "" "" "SOURCES" ${ARGN})

  if(NOT ARG_SOURCES)
    message(FATAL_ERROR "mini_bt_add_level_executable(${target_name}): SOURCES is required")
  endif()

  foreach(_src IN LISTS ARG_SOURCES)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_src}")
      message(FATAL_ERROR
        "${target_name}: missing source ${CMAKE_CURRENT_SOURCE_DIR}/${_src}")
    endif()
  endforeach()

  add_executable(${target_name} ${ARG_SOURCES})
  target_compile_features(${target_name} PRIVATE cxx_std_17)

  if(MSVC)
    target_compile_options(${target_name} PRIVATE /utf-8)
  endif()

  set(_gen_inc "${CMAKE_CURRENT_BINARY_DIR}/generated_include")
  file(REMOVE_RECURSE "${_gen_inc}")
  file(MAKE_DIRECTORY "${_gen_inc}/mini_bt")

  file(GLOB _level_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
  foreach(_hdr IN LISTS _level_headers)
    get_filename_component(_name "${_hdr}" NAME)
    configure_file("${_hdr}" "${_gen_inc}/mini_bt/${_name}" COPYONLY)
  endforeach()

  # Only generated_include: forces #include "mini_bt/xxx.hpp".
  # Do NOT add CMAKE_CURRENT_SOURCE_DIR — mixing "xxx.hpp" and "mini_bt/xxx.hpp"
  # treats the copy as a second file and causes type redefinition errors.
  target_include_directories(${target_name} PRIVATE "${_gen_inc}")
endfunction()
