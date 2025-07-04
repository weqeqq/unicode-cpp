
cmake_minimum_required(VERSION 3.23)

project(unicode CXX)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)
if(BUILD_SHARED_LIBS)
    add_library(unicode SHARED)
else()
    add_library(unicode STATIC)
endif()
target_compile_features(unicode PUBLIC cxx_std_17)
target_sources(unicode PUBLIC
    FILE_SET HEADERS BASE_DIRS
        headers
    FILES
        headers/unicode/convert.h
        headers/unicode/error.h
        headers/unicode/length.h
        headers/unicode/type.h
    PRIVATE
        sources/convert.cc
        sources/length.cc
)
option(UNICODE_USE_SIMDUTF "" ON)
if(UNICODE_USE_SIMDUTF)
    include(FetchContent)
    FetchContent_Declare(
        dependency-cmake
        GIT_REPOSITORY https://github.com/weqeqq/dependency-cmake
        GIT_TAG main
    )
    FetchContent_MakeAvailable(dependency-cmake)
    Dependency(simdutf/simdutf dde0ddd293069cd4a44675d8e43e59f3d37ff7c5
        EXPECTED
            lib/libsimdutf.a
    )
    target_link_libraries(unicode
        PRIVATE
            unicode::external::simdutf)
    target_compile_definitions(unicode
        PRIVATE
            UNICODE_USE_SIMDUTF
    )
endif()
add_library(unicode::unicode ALIAS unicode)

option(UNICODE_BUILD_TESTS "" ${PROJECT_IS_TOP_LEVEL})
if(UNICODE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS unicode
    EXPORT unicode-targets
    FILE_SET HEADERS
)
install(EXPORT unicode-targets
    FILE unicode-targets.cmake
    NAMESPACE unicode::
    DESTINATION lib/cmake/unicode
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/unicode-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/unicode-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/unicode-config.cmake
    INSTALL_DESTINATION lib/cmake/unicode
)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/unicode-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/unicode-config-version.cmake
    DESTINATION lib/cmake/unicode
)
