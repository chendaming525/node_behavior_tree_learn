cmake_minimum_required(VERSION 3.16)
mini_bt_add_level_executable(level_l14_xml_graduate SOURCES l14_xml_graduate.cpp)
target_link_libraries(level_l14_xml_graduate PRIVATE tinyxml2::tinyxml2)
target_compile_definitions(level_l14_xml_graduate PRIVATE
  L14_ENTER_ROOM_XML="${CMAKE_CURRENT_SOURCE_DIR}/l14_enter_room.xml"
)
