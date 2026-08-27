cmake_minimum_required(VERSION 3.16)

# L10 directly includes headers from this directory. It does not use the
# generated mini_bt/ aliases and never searches full/.
add_executable(level_l10_factory l10_factory.cpp)
target_compile_features(level_l10_factory PRIVATE cxx_std_17)
target_include_directories(level_l10_factory PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
  target_compile_options(level_l10_factory PRIVATE /utf-8)
endif()
