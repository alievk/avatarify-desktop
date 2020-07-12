cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "avatarify")
project(${PROJECT})

set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt/5.15.0/lib/cmake;/Users/vlivashkin/Documents/GitHub/avatarify-desktop-cpp/lib/libtorch")

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(BUILD_CAFFE2_OPS OFF)

find_package(Qt5 COMPONENTS Widgets Qml Quick Multimedia Gui REQUIRED)  # Find the Qt libraries for Qt Quick/QML
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}} -DQT_NO_KEYWORDS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14 -fstrict-aliasing -pedantic-errors -pedantic -Wno-deprecated-declarations -Wno-unused-variable")
set(CMAKE_INCLUDE_CURRENT_DIR ON)  # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON)  # Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOUIC ON)  # if you have any .ui files
set(CMAKE_AUTORCC ON)  # if you have any .qrc files

qt5_add_resources(QT_RESOURCES src/qml.qrc) # add the resources.qrc file
qt5_add_resources(QT_RESOURCES resources/resources.qrc) # add the resources.qrc file
#qt5_create_translation(QT_RESOURCES ./src/avatarify-desktop_en_US.ts)

set(HEADERS ${HEADERS}
        src/camera/AsyncCameraCapture.h
        src/camera/PoorMansProbe.h
        src/predictors/Predictor.h
        src/predictors/IdentityPredictor.h
        src/predictors/LibtorchPredictor.h
        src/predictors/LibtorchIdentityPredictor.h
        src/predictors/LibtorchFOMM.h
        src/vcam/AbstractVCam.h
        src/vcam/DirectShowVCam.h
        src/vcam/StubVCam.h
        src/InferenceManager.h
        src/InferenceWorker.h
        )
set(SOURCES ${SOURCES}
        src/camera/AsyncCameraCapture.cpp
        src/camera/PoorMansProbe.cpp
        src/predictors/Predictor.cpp
        src/predictors/IdentityPredictor.cpp
        src/predictors/LibtorchPredictor.cpp
        src/predictors/LibtorchIdentityPredictor.cpp
        src/predictors/LibtorchFOMM.cpp
        src/vcam/AbstractVCam.cpp
        src/vcam/DirectShowVCam.cpp
        src/vcam/StubVCam.cpp
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
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)

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