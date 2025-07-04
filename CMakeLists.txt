
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
target_compile_features(unicode PUBLIC cxx_std_23)
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
        simdutf
        GIT_REPOSITORY https://github.com/simdutf/simdutf
        GIT_TAG dde0ddd293069cd4a44675d8e43e59f3d37ff7c5
    )
    FetchContent_MakeAvailable(simdutf)
    target_link_libraries(unicode
        PRIVATE
            simdutf::simdutf
    )
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
