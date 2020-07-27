cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "vcam-driver")
project(${PROJECT})

message("  -- build ${PROJECT}")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(WDK REQUIRED)

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
        avshws.h
        avshws.inx
        avshws.rc
        capture.cpp
        capture.h
        device.cpp
        device.h
        filter.cpp
        filter.h
        hwsim.cpp
        hwsim.h
        image.cpp
        image.h
        purecall.c
        )

set(CMAKE_ENABLE_EXPORTS TRUE)
set(BUILD_SHARED_LIBS TRUE)
wdk_add_driver(${PROJECT}
        ${SOURCES}
        )
target_include_directories(${PROJECT} PRIVATE
        ${Boost_INCLUDE_DIRS}
        )
target_link_libraries(${PROJECT}
        ${Boost_LIBRARIES}
        )
