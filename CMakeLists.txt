cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(PROJECT "Avatarify")
project(${PROJECT})
message("-- build ${PROJECT}")

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach (CompilerFlag ${CompilerFlags})
    string(REPLACE "/MT" "/MD" ${CompilerFlag} "${${CompilerFlag}}")
endforeach ()

set(VERBOSE 1)  # for whom it may concern

add_subdirectory(lib/akvirtualcamera)

# libraries paths
if (MSVC)
    set(QT_INSTALL_PREFIX "C:\\Qt\\5.15.0\\msvc2019_64\\lib\\cmake")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(TORCH_INSTALL_PREFIX "C:\\SDKs\\libtorch-win-shared-with-deps-debug-1.6.0\\libtorch")
    else ()
        set(TORCH_INSTALL_PREFIX "C:\\SDKs\\libtorch-win-shared-with-deps-1.6.0\\libtorch")
    endif ()
else ()
    set(QT_INSTALL_PREFIX "/usr/local/Cellar/qt/5.15.0/lib/cmake")
    set(TORCH_INSTALL_PREFIX "./lib/libtorch")
endif ()
set(TORCH_CMAKE "${TORCH_INSTALL_PREFIX}/share/cmake/Torch")
set(CMAKE_PREFIX_PATH "${QT_INSTALL_PREFIX};${TORCH_CMAKE}")


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


# libtorch
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(BUILD_CAFFE2_OPS OFF)


# libyuv
add_library(libyuv STATIC IMPORTED)
set_target_properties(libyuv PROPERTIES
        IMPORTED_LOCATION_DEBUG "C:\\SDKs\\libyuv\\src\\cmake-build-debug\\yuv.lib"
        IMPORTED_LOCATION_RELEASE "C:\\SDKs\\libyuv\\src\\cmake-build-release\\yuv.lib"
        INTERFACE_INCLUDE_DIRECTORIES "C:\\SDKs\\libyuv\\src\\include"
        )


set(HEADERS ${HEADERS}
        src/camera/AsyncCameraCapture.h
        src/camera/PoorMansProbe.h
        src/predictors/Predictor.h
        src/predictors/IdentityPredictor.h
        src/predictors/LibtorchPredictor.h
        src/predictors/LibtorchIdentityPredictor.h
        src/predictors/LibtorchFOMM.h
        src/vcam/AbstractVCamInterface.h
        src/vcam/AkVCamBridge.h
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
        src/vcam/AbstractVCamInterface.cpp
        src/vcam/AkVCamBridge.cpp
        src/vcam/StubVCam.cpp
        src/InferenceManager.cpp
        src/InferenceWorker.cpp
        src/main.cpp
        )
add_executable(${PROJECT}
        ${HEADERS}
        ${SOURCES}
        ${QT_RESOURCES}
        )
target_include_directories(${PROJECT} PRIVATE
        libyuv
        ${TORCH_INSTALL_PREFIX}/include
)
target_link_libraries(${PROJECT} PUBLIC
        Qt5::Widgets
        Qt5::Qml
        Qt5::Quick
        Qt5::Multimedia
        ${TORCH_LIBRARIES}
        VCamUtils
        VCamIPC
        libyuv
        )
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)


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

    # copy QT dlls
    include(cmake/windeployqt.cmake)
    windeployqt(${PROJECT})
endif (MSVC)


#set(CPACK_PACKAGE_NAME "MyPackage")
#set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MyPackage Installation Example")
#set(CPACK_PACKAGE_VERSION "1.0.0") # Version of installer
#
#include(CPack)
#include(CPackIFW)
#
#cpack_add_component(${PROJECT}
#        DISPLAY_NAME "MyApp"
#        DESCRIPTION "My Application")
#cpack_ifw_configure_component(${PROJECT}
#        VERSION "1.2.3" # Version of component
#        SCRIPT "operations.qs")
#cpack_add_component(mybigplugin
#        DISPLAY_NAME "MyBigPlugin"
#        DESCRIPTION "My Big Downloadable Plugin"
#        DOWNLOADED)
#cpack_ifw_add_repository(myrepo
#        URL "http://example.com/ifw/repo/myapp"
#        DISPLAY_NAME "My Application Repository")