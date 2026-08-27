cmake_minimum_required(VERSION 3.16)
mini_bt_add_level_executable(level_l13_subtree SOURCES l13_subtree.cpp)
target_link_libraries(level_l13_subtree PRIVATE tinyxml2::tinyxml2)
target_compile_definitions(level_l13_subtree PRIVATE
  L13_SUBTREE_XML="${CMAKE_CURRENT_SOURCE_DIR}/l13_subtree.xml"
)
