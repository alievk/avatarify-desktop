cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "DShowVCam")
project(${PROJECT})

message("  -- build ${PROJECT}")

add_subdirectory(baseclasses)


# Boost
set(Boost_INCLUDE_DIR "C:\\SDKs\\boost_1_72_0")
set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.72.0 COMPONENTS date_time REQUIRED)
#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)


set(SOURCES
        CVCamFilter.cpp
        CVCamFilter.h
        CVCamOutStream.cpp
        CVCamOutStream.h
        dllmain.cpp
        DSVCam.def
        stdafx.h
        utils/Log.h
        )

set(CMAKE_ENABLE_EXPORTS TRUE)
set(BUILD_SHARED_LIBS TRUE)
add_library(${PROJECT} SHARED
        ${SOURCES}
        baseclasses
        )
target_include_directories(${PROJECT} PRIVATE
        ${Boost_INCLUDE_DIRS}
        )
target_link_libraries(${PROJECT}
        ${Boost_LIBRARIES}
        )

if (MSVC)
    set_target_properties(${PROJECT} PROPERTIES COMPILE_OPTIONS "/Zc:wchar_t-")
endif()
