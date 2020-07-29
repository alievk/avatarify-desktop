cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "avatarify")
project(${PROJECT})
message("-- build ${PROJECT}")

# virtual camera
if (MSVC)
    add_subdirectory(lib/ds_vcam)
endif ()


# libraries paths
if (MSVC)
    set(QT_INSTALL_PREFIX "C:\\Qt\\5.15.0\\msvc2019_64\\lib\\cmake")
    set(Boost_INCLUDE_DIR "C:\\SDKs\\boost_1_72_0")
else ()
    set(QT_INSTALL_PREFIX "/usr/local/Cellar/qt/5.15.0/lib/cmake")
    set(Boost_INCLUDE_DIR "/opt/boost/boost_1_72_0")
endif ()
set(TORCH_INSTALL_PREFIX "./lib/libtorch")
set(CMAKE_PREFIX_PATH "${QT_INSTALL_PREFIX};${TORCH_INSTALL_PREFIX}")


# Qt
find_package(Qt5 COMPONENTS Widgets Qml Quick Multimedia Gui REQUIRED)  # Find the Qt libraries for Qt Quick/QML
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}} -DQT_NO_KEYWORDS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)  # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON)  # Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOUIC ON)  # if you have any .ui files
set(CMAKE_AUTORCC ON)  # if you have any .qrc files

qt5_add_resources(QT_RESOURCES src/qml.qrc) # add the resources.qrc file
qt5_add_resources(QT_RESOURCES resources/resources.qrc) # add the resources.qrc file
#qt5_create_translation(QT_RESOURCES ./src/avatarify-desktop_en_US.ts)


# libtorch
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(BUILD_CAFFE2_OPS OFF)


# Boost
set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.72.0 COMPONENTS date_time REQUIRED)

set(HEADERS ${HEADERS}
        src/camera/AsyncCameraCapture.h
        src/camera/PoorMansProbe.h
        src/camera/yuv2rgb.h
        src/predictors/Predictor.h
        src/predictors/IdentityPredictor.h
        src/predictors/LibtorchPredictor.h
        src/predictors/LibtorchIdentityPredictor.h
        src/predictors/LibtorchFOMM.h
        src/vcam/AbstractVCam.h
        src/vcam/StubVCam.h
        src/vcam/windows/DriverVCam.h
        src/vcam/windows/DirectShowVCam.h
        src/InferenceManager.h
        src/InferenceWorker.h
        )
set(SOURCES ${SOURCES}
        src/camera/AsyncCameraCapture.cpp
        src/camera/PoorMansProbe.cpp
        src/camera/yuv2rgb.cpp
        src/predictors/Predictor.cpp
        src/predictors/IdentityPredictor.cpp
        src/predictors/LibtorchPredictor.cpp
        src/predictors/LibtorchIdentityPredictor.cpp
        src/predictors/LibtorchFOMM.cpp
        src/vcam/AbstractVCam.cpp
        src/vcam/StubVCam.cpp
        src/vcam/windows/DriverVCam.cpp
        src/vcam/windows/DirectShowVCam.cpp
        src/InferenceManager.cpp
        src/InferenceWorker.cpp
        src/main.cpp
        )
add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${QT_RESOURCES})
target_include_directories(${PROJECT} PRIVATE
        ${Boost_INCLUDE_DIRS}
        )
target_link_libraries(${PROJECT}
        Qt5::Widgets
        Qt5::Qml
        Qt5::Quick
        Qt5::Multimedia
        ${TORCH_LIBRARIES}
        ${Boost_LIBRARIES}
        )
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)

if (MSVC)
    #    target_compile_options(${PROJECT} PRIVATE /W4 /WX)
else ()
    target_compile_options(${PROJECT} PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET ${PROJECT}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS}
            $<TARGET_FILE_DIR:${PROJECT}>)
endif (MSVC)