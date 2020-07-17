cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "SVCam")
project(${PROJECT})

message("  -- build ${PROJECT}")

include_directories("${PROJECT_SOURCE_DIR}/lib/svcam/baseclasses")
add_subdirectory(baseclasses)

set(SOURCES
        dll.def
        dllmain.cpp
        stdafx.cpp
        stdafx.h
        SVCam.cpp
        SVCam.h
        targetver.h
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
