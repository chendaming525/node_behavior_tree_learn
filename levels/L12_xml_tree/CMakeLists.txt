cmake_minimum_required(VERSION 3.16)
mini_bt_add_level_executable(level_l12_xml_tree
  SOURCES
    l12_xml_tree.cpp
)
target_link_libraries(level_l12_xml_tree PRIVATE tinyxml2::tinyxml2)
target_compile_definitions(level_l12_xml_tree PRIVATE
  L12_ENTER_ROOM_XML="${CMAKE_CURRENT_SOURCE_DIR}/l12_enter_room.xml"
)
