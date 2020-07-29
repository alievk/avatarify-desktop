cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "VCam-src")
project(${PROJECT})

message("  -- build ${PROJECT}")

add_subdirectory(baseclasses)

set(SOURCES
        Dll.cpp
        Filters.cpp
        Filters.def
        Filters.h
        )

set(CMAKE_ENABLE_EXPORTS TRUE)
set(BUILD_SHARED_LIBS TRUE)
add_library(${PROJECT} SHARED
        ${SOURCES}
        baseclasses
        )

if (MSVC)
    set_target_properties(${PROJECT} PROPERTIES COMPILE_OPTIONS "/Zc:wchar_t-")
endif()
