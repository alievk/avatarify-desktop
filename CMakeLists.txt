cmake_minimum_required(VERSION 3.5)

set(PROJECT "avatarify")
project(${PROJECT})

find_package(Qt5 COMPONENTS Widgets Qml Quick Multimedia Gui REQUIRED)  # Find the Qt libraries for Qt Quick/QML
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14 -fstrict-aliasing -pedantic-errors -pedantic -Wno-deprecated-declarations -Wno-unused-variable")
set(CMAKE_INCLUDE_CURRENT_DIR ON)  # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON)  # Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOUIC ON)  # if you have any .ui files
set(CMAKE_AUTORCC ON)  # if you have any .qrc files

#find_package(Torch REQUIRED)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

qt5_add_resources(QT_RESOURCES src/qml.qrc) # add the qml.qrc file
#qt5_create_translation(QT_RESOURCES ./src/avatarify-desktop_en_US.ts)

set(HEADERS ${HEADERS}
        src/AsyncCameraCapture.h
        src/PoorMansProbe.h
        src/predictors/Predictor.h
        src/predictors/IdentityPredictor.h
#        src/predictors/LibtorchPredictor.h
        src/InferenceManager.h
        src/InferenceWorker.h
        )
set(SOURCES ${SOURCES}
        src/AsyncCameraCapture.cpp
        src/PoorMansProbe.cpp
        src/predictors/Predictor.cpp
        src/predictors/IdentityPredictor.cpp
#        src/predictors/LibtorchPredictor.cpp
        src/InferenceManager.cpp
        src/InferenceWorker.cpp
        src/main.cpp
        )
add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${QT_RESOURCES})
target_link_libraries(${PROJECT}
        Qt5::Widgets
        Qt5::Qml
        Qt5::Quick
        Qt5::Multimedia
        ${TORCH_LIBRARIES}
        )

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET example-app
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS}
            $<TARGET_FILE_DIR:example-app>)
endif (MSVC)