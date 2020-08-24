cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
include(FetchContent)

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
if (WIN32)
    if (MSVC)
        set(QTDIR "C:\\Qt\\5.15.0\\msvc2019_64")
    elseif (MINGW)
        set(QTDIR "C:\\Qt\\5.15.0\\mingw81_64")
    endif ()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(TORCHDIR "C:\\SDKs\\libtorch-win-shared-with-deps-debug-1.6.0\\libtorch")
    else ()
        set(TORCHDIR "C:\\SDKs\\libtorch-win-shared-with-deps-1.6.0\\libtorch")
    endif ()
elseif (APPLE)
    set(QTDIR "/usr/local/Cellar/qt/5.15.0")
    set(TORCHDIR "./lib/libtorch")
elseif (UNIX)
    set(QTDIR "/usr/local/Cellar/qt/5.15.0")
    set(TORCHDIR "./lib/libtorch")
endif ()
set(ENV{QTDIR} ${QTDIR})
set(QT_CMAKE "${QTDIR}/lib/cmake")
set(TORCH_CMAKE "${TORCHDIR}/share/cmake/Torch")
set(CMAKE_PREFIX_PATH "${QT_CMAKE};${TORCH_CMAKE}")


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


# dlib
set(DLIB_USE_CUDA 0)
#add_compile_definitions(DLIB_PNG_SUPPORT)
add_library(dlib STATIC IMPORTED)
set_target_properties(dlib PROPERTIES
        IMPORTED_LOCATION_DEBUG "C:\\SDKs\\dlib-19.21\\cmake-build-debug\\dlib\\dlib19.21.0_debug_64bit_msvc1926.lib"
        IMPORTED_LOCATION_RELEASE "C:\\SDKs\\dlib-19.21\\cmake-build-release\\dlib\\dlib19.21.0_release_64bit_msvc1926.lib"
        INTERFACE_INCLUDE_DIRECTORIES "C:\\SDKs\\dlib-19.21"
        )


# eigen3
set(EIGEN_INC_DIR C:\\SDKs\\eigen3)
include_directories(${EIGEN_INC_DIR})


set(HEADERS ${HEADERS}
        src/camera/crop/DlibShapePredictor.h
        src/camera/crop/KalmanCrop.h
        src/camera/crop/KalmanFilter.h
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
        src/camera/crop/DlibShapePredictor.cpp
        src/camera/crop/KalmanCrop.cpp
        src/camera/crop/KalmanFilter.cpp
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
        "C:/SDKs/dlib-19.21/dlib/all/source.cpp"
        src/camera/crop/KalmanFilter1d.cpp src/camera/crop/KalmanFilter1d.h)
add_executable(${PROJECT}
        ${HEADERS}
        ${SOURCES}
        ${QT_RESOURCES}
        )
target_include_directories(${PROJECT} PRIVATE
        ${TORCH_INSTALL_PREFIX}/include
        libyuv
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
        dlib
        )
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)


if (MSVC OR MINGW)
    # copy QT dlls
    include(cmake/windeployqt.cmake)
    windeployqt(${PROJECT})

    # Copy libtorch dlls (https://github.com/pytorch/pytorch/issues/25457)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET ${PROJECT}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS}
            $<TARGET_FILE_DIR:${PROJECT}>)
endif ()


# INSTALLER
set(CPACK_IFW_ROOT "./AfyD_installer")
install(
        TARGETS ${PROJECT}
        RUNTIME DESTINATION ${PROJECT}
        COMPONENT qt_cpackifw_installer
        BUNDLE DESTINATION ${PROJECT}
        COMPONENT qt_cpackifw_installer
)

function(CPACKIFW_COMMON)
    set(CPACK_PACKAGE_NAME ${PROJECT})
    set(CPACK_PACKAGE_FILE_NAME installer)
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Installation Tool")
    set(CPACK_PACKAGE_VERSION "1.0.0") # Version of installer
    set(CPACK_COMPONENTS_ALL qt_cpackifw_installer)
    set(CPACK_IFW_PACKAGE_START_MENU_DIRECTORY ${PROJECT})
    set(CPACK_GENERATOR IFW)
    set(CPACK_IFW_VERBOSE ON)

    include(CPack REQUIRED)
    include(CPackIFW REQUIRED)

    cpack_add_component(
            qt_cpackifw_installer
            DISPLAY_NAME "Qt CPackIFW"
            DESCRIPTION "Install me"
            REQUIRED
    )

    cpack_ifw_configure_component(
            qt_cpackifw_installer
            FORCED_INSTALLATION
            NAME qt.cpackifw.installer
            VERSION ${PROJECT_VERSION} # Version of component
            LICENSES License ${qt_cpackifw_SOURCE_DIR}/LICENSE
            DEFAULT TRUE
    )
endfunction()

if(CPACK_IFW_ROOT OR DEFINED ENV{QTIFWDIR})
    if(DEFINED ENV{QTDIR})
        if(APPLE)
            if(EXISTS $ENV{QTDIR}/bin/macdeployqt)
                message("*")
                message("* Note")
                message("*")
                message("* Because of a bug in CPackIFW, it doesn't manage correctly the package created on OSX.")
                message("* Unfortunately CPackIFW doesn't forward the .dmg extension to the binarycreator (see IFW documentation for more details).")
                message("* Therefore it creates a .app directory on OSX, that isn't properly a file and cannot be treated the same way .run and .exe files are.")
                message("* As a result, make package generates an empty directory within the build directory and leaves the bundle somewhere within the _CPack_Packages directory.")
                message("*")
                message("* I strongly suggest to run binarycreator through an external script and use the .dmg extension directly. At least until the bug in CPackIFW isn't fixed.")
                message("* From within the build directory run:")
                message("*")
                message("*     binarycreator -f -c _CPack_Packages/Darwin/IFW/installer/config/config.xml -p _CPack_Packages/Darwin/IFW/installer/packages/ package/installer.dmg")
                message("*")
                message("* The bundle and the dmg file will be created within the package directory as:")
                message("*")
                message("*     package/installer.app [bundle]")
                message("*     package/installer.dmg [dmg]")
                message("*")

                add_custom_command(
                        TARGET ${PROJECT} POST_BUILD
                        COMMAND $ENV{QTDIR}/bin/macdeployqt ${PROJECT_NAME}.app
                )

                CPACKIFW_COMMON()
            else()
                message("Unable to find executable QTDIR/bin/macdeployqt.")
            endif()
        elseif(WIN32)
            if(EXISTS $ENV{QTDIR}/bin/windeployqt.exe)
                add_custom_command(
                        TARGET ${PROJECT} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/windeployqt_stuff
                        COMMAND $ENV{QTDIR}/bin/windeployqt.exe --compiler-runtime --dir ${CMAKE_BINARY_DIR}/windeployqt_stuff $<TARGET_FILE:${PROJECT}>
                )

                install(
                        DIRECTORY ${CMAKE_BINARY_DIR}/windeployqt_stuff/
                        DESTINATION ${PROJECT_NAME}
                        COMPONENT qt_cpackifw_installer
                )

                CPACKIFW_COMMON()
            else()
                message("Unable to find executable QTDIR/bin/windeployqt.")
            endif()
        elseif(UNIX)
            CPACKIFW_COMMON()
        endif()
    else()
        message("Set properly environment variable QTDIR to be able to create a package.")
    endif()
else()
    message("If you want to enable target package you can:")
    message("\t* Either pass -DCPACK_IFW_ROOT=<path> to cmake")
    message("\t* Or set the environment variable QTIFWDIR")
    message("To specify the location of the QtIFW tool suite.")
    message("The specified path should not contain bin at the end (for example: D:\\DevTools\\QtIFW2.0.5).")
endif()
