cmake_minimum_required(VERSION 3.16)
mini_bt_add_level_executable(level_l11_parse_xml SOURCES l11_parse_xml.cpp)
target_link_libraries(level_l11_parse_xml PRIVATE tinyxml2::tinyxml2)
target_compile_definitions(level_l11_parse_xml PRIVATE
  L11_ENTER_ROOM_XML="${CMAKE_CURRENT_SOURCE_DIR}/enter_room.xml"
)
