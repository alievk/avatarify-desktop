cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "avatarify")
project(${PROJECT})

if(MSVC)
    set(QT_INSTALL_PREFIX "C:\\Qt\\5.15.0\\msvc2019_64\\lib\\cmake")
else()
    set(QT_INSTALL_PREFIX "/usr/local/Cellar/qt/5.15.0/lib/cmake")
endif()
set(TORCH_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/lib/libtorch")
set(CMAKE_PREFIX_PATH "${QT_INSTALL_PREFIX};${TORCH_INSTALL_PREFIX}")

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(BUILD_CAFFE2_OPS OFF)

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

set(WINSDK71_SOURCES
        src/vcam/baseclasses/amfilter.cpp
        src/vcam/baseclasses/amvideo.cpp
        src/vcam/baseclasses/arithutil.cpp
        src/vcam/baseclasses/combase.cpp
        src/vcam/baseclasses/cprop.cpp
        src/vcam/baseclasses/ctlutil.cpp
        src/vcam/baseclasses/ddmm.cpp
        src/vcam/baseclasses/dllentry.cpp
        src/vcam/baseclasses/dllsetup.cpp
        src/vcam/baseclasses/mtype.cpp
        src/vcam/baseclasses/outputq.cpp
        src/vcam/baseclasses/perflog.cpp
        src/vcam/baseclasses/pstream.cpp
        src/vcam/baseclasses/pullpin.cpp
        src/vcam/baseclasses/refclock.cpp
        src/vcam/baseclasses/renbase.cpp
        src/vcam/baseclasses/schedule.cpp
        src/vcam/baseclasses/seekpt.cpp
        src/vcam/baseclasses/source.cpp
        src/vcam/baseclasses/stdafx.cpp
        src/vcam/baseclasses/strmctl.cpp
        src/vcam/baseclasses/sysclock.cpp
        src/vcam/baseclasses/transfrm.cpp
        src/vcam/baseclasses/transip.cpp
        src/vcam/baseclasses/videoctl.cpp
        src/vcam/baseclasses/vtrans.cpp
        src/vcam/baseclasses/winctrl.cpp
        src/vcam/baseclasses/winutil.cpp
        src/vcam/baseclasses/wxdebug.cpp
        src/vcam/baseclasses/wxlist.cpp
        src/vcam/baseclasses/wxutil.cpp

        )


set(WINSDK71
        src/vcam/baseclasses/amextra.h
        src/vcam/baseclasses/amfilter.h
        src/vcam/baseclasses/cache.h
        src/vcam/baseclasses/checkbmi.h
        src/vcam/baseclasses/combase.h
        src/vcam/baseclasses/cprop.h
        src/vcam/baseclasses/ctlutil.h
        src/vcam/baseclasses/ddmm.h
        src/vcam/baseclasses/dllsetup.h
        src/vcam/baseclasses/dxmperf.h
        src/vcam/baseclasses/fourcc.h
        src/vcam/baseclasses/measure.h
        src/vcam/baseclasses/msgthrd.h
        src/vcam/baseclasses/mtype.h
        src/vcam/baseclasses/outputq.h
        src/vcam/baseclasses/perflog.h
        src/vcam/baseclasses/perfstruct.h
        src/vcam/baseclasses/pstream.h
        src/vcam/baseclasses/pullpin.h
        src/vcam/baseclasses/refclock.h
        src/vcam/baseclasses/reftime.h
        src/vcam/baseclasses/renbase.h
        src/vcam/baseclasses/schedule.h
        src/vcam/baseclasses/seekpt.h
        src/vcam/baseclasses/source.h
        src/vcam/baseclasses/stdafx.h
        src/vcam/baseclasses/streams.h
        src/vcam/baseclasses/strmctl.h
        src/vcam/baseclasses/sysclock.h
        src/vcam/baseclasses/transfrm.h
        src/vcam/baseclasses/transip.h
        src/vcam/baseclasses/videoctl.h
        src/vcam/baseclasses/vtrans.h
        src/vcam/baseclasses/winctrl.h
        src/vcam/baseclasses/winutil.h
        src/vcam/baseclasses/wxdebug.h
        src/vcam/baseclasses/wxlist.h
        src/vcam/baseclasses/wxutil.h
        )

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
        src/vcam/DirectShowVCam.h
        src/vcam/StubVCam.h
        src/vcam/svcam/stdafx.h
        src/vcam/svcam/SVCam.h
        src/vcam/svcam/targetver.h
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
        src/vcam/DirectShowVCam.cpp
        src/vcam/StubVCam.cpp
        src/vcam/svcam/dllmain.cpp
        src/vcam/svcam/stdafx.cpp
        src/vcam/svcam/SVCam.cpp
        src/InferenceManager.cpp
        src/InferenceWorker.cpp
        src/main.cpp
        )
add_executable(${PROJECT} ${WINSDK71} ${HEADERS} ${SOURCES} ${QT_RESOURCES})
target_link_libraries(${PROJECT}
        Qt5::Widgets
        Qt5::Qml
        Qt5::Quick
        Qt5::Multimedia
        ${TORCH_LIBRARIES}
        )
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)

if(MSVC)
#    target_compile_options(${PROJECT} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

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