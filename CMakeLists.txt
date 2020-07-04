cmake_minimum_required(VERSION 3.5)

set(PROJECT "avatarify")
project(${PROJECT})

find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -fstrict-aliasing -pedantic-errors -pedantic -Wno-deprecated-declarations -Wno-unused-variable")
set(CMAKE_INCLUDE_CURRENT_DIR ON)  # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON)  # Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOUIC ON)  # if you have any .ui files
set(CMAKE_AUTORCC ON)  # if you have any .qrc files

find_package(Qt5 REQUIRED Qml Quick Gui)  # Find the Qt libraries for Qt Quick/QML

qt5_add_resources(QT_RESOURCES ./src/qml.qrc) # add the qml.qrc file

set(HEADERS ${HEADERS}
        ./src/backend.h
        )
set(SOURCES ${SOURCES}
        ./src/backend.cpp
        ./src/main.cpp
        )
add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${QT_RESOURCES})
target_link_libraries(${PROJECT}
        Qt5::Widgets
        Qt5::Qml
        Qt5::Quick
        )