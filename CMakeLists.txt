
cmake_minimum_required(VERSION 3.23)

project(unicode LANGUAGES CXX VERSION 0.2.0)

add_library(unicode)
add_library(unicode::unicode ALIAS unicode)

include(FetchContent)

option(UNICODE_USE_SIMDUTF "Use simdutf as unicode backend" ON)

set(FETCHCONTENT_QUIET OFF)
if(UNICODE_USE_SIMDUTF)
  FetchContent_Declare(
    simdutf-project

    GIT_REPOSITORY https://github.com/weqeqq/simdutf
    GIT_TAG 3e9a394f6e71e835fb5f8eab90becc9f8a5be3e2
    GIT_PROGRESS ON
  )
endif()
FetchContent_Declare(
  exception-project

  GIT_REPOSITORY https://github.com/weqeqq/exception-cpp.git
  GIT_TAG 42843f32621a206b999bac52d25f6ca0d821f440
  GIT_PROGRESS ON
)
FetchContent_MakeAvailable(
  exception-project
)
if (UNICODE_USE_SIMDUTF)
  FetchContent_MakeAvailable(
    simdutf-project
  )
  target_link_libraries(unicode 
    PRIVATE 
    simdutf::simdutf
  )
endif()
target_link_libraries(unicode 
  PUBLIC
  exception::exception
)
target_sources(unicode PUBLIC 
  FILE_SET  HEADERS 
  BASE_DIRS include
  FILES 
  include/unicode/convert.h
  include/unicode/error.h 
  include/unicode/length.h
  include/unicode/type.h

  PRIVATE 
  source/convert.cc
  source/length.cc
)
install(
  TARGETS unicode 
  EXPORT  unicode-targets FILE_SET HEADERS
)
install(
  EXPORT    unicode-targets 
  NAMESPACE unicode:: DESTINATION lib/cmake/unicode
)
include(CMakePackageConfigHelpers)

set(UNICODE_INCLUDE_DIR include)
set(UNICODE_LIBRARY_DIR lib)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/unicode-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/unicode-config.cmake
  INSTALL_DESTINATION lib/cmake/unicode
  PATH_VARS 
    UNICODE_INCLUDE_DIR 
    UNICODE_LIBRARY_DIR
)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/unicode-config-version.cmake COMPATIBILITY SameMajorVersion
)
install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/unicode-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/unicode-config-version.cmake
  DESTINATION lib/cmake/unicode
)
