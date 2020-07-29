cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "DShowVCamAssistant")
project(${PROJECT})

message("  -- build ${PROJECT}")


set(SOURCES
        main.cpp
        service.cpp
        service.h
        )

add_executable(${PROJECT}
        ${SOURCES}
        )
