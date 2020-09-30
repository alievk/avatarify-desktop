TORCHDIR = c:/SDKs/libtorch-win-shared-with-deps-1.6.0
TORCHDIR_DEBUG = c:/SDKs/libtorch-win-shared-with-deps-debug-1.6.0

QT += quick \
    widgets \
    quickcontrols2 \
    multimedia

CONFIG += c++17 \
    qmltypes

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/camera/AsyncCameraCapture.h \
    src/camera/PoorMansProbe.h \
    src/camera/crop/KalmanCrop.h \
    src/camera/crop/KalmanFilter.h \
    src/camera/crop/KalmanFilter1d.h \
    src/camera/crop/DlibShapePredictor.h \
    src/predictors/IdentityPredictor.h \
    src/predictors/LibtorchFOMM.h \
    src/predictors/LibtorchIdentityPredictor.h \
    src/predictors/LibtorchPredictor.h \
    src/predictors/Predictor.h \
    src/utils/cipherbuf.h \
    src/utils/cipherreader.h \
    src/utils/utils.h \
    src/vcam/AbstractVCamInterface.h \
    src/vcam/AkVCamBridge.h \
    src/vcam/StubVCam.h \
    src/InferenceManager.h \
    src/InferenceWorker.h \

# for dlib to compile
QMAKE_CXXFLAGS += /bigobj
SOURCES += \
    src/camera/AsyncCameraCapture.cpp \
    src/camera/PoorMansProbe.cpp \
    src/camera/crop/KalmanCrop.cpp \
    src/camera/crop/KalmanFilter.cpp \
    src/camera/crop/KalmanFilter1d.cpp \
    src/camera/crop/DlibShapePredictor.cpp \
    src/predictors/IdentityPredictor.cpp \
    src/predictors/LibtorchFOMM.cpp \
    src/predictors/LibtorchIdentityPredictor.cpp \
    src/predictors/LibtorchPredictor.cpp \
    src/predictors/Predictor.cpp \
    src/utils/cipherbuf.cpp \
    src/utils/cipherreader.cpp \
    src/utils/utils.cpp \
    src/vcam/AbstractVCamInterface.cpp \
    src/vcam/AkVCamBridge.cpp \
    src/vcam/StubVCam.cpp \
    src/InferenceManager.cpp \
    src/InferenceWorker.cpp \
    src/main.cpp

RESOURCES += src/qml.qrc

OTHER_FILES = src/main.qml \
    src/qtquickcontrols2.conf

TRANSLATIONS += \
    src/avatarify-desktop_en_US.ts

QMAKE_CXXFLAGS += -DGLIBCXX_USE_CXX11_ABI=0
CONFIG += no_keywords

# libtorch
INCLUDEPATH += $${TORCHDIR}/include
INCLUDEPATH += $${TORCHDIR}/include/torch/csrc/api/include
win32:CONFIG(release, debug|release): {
    LIBS += -L"$${TORCHDIR}/lib"
}
else:win32:CONFIG(debug, debug|release): {
    LIBS += -L"$${TORCHDIR_DEBUG}/lib"
}
LIBS += \
    asmjit.lib \
    c10.lib \
    c10_cuda.lib \
    caffe2_detectron_ops_gpu.lib \
    caffe2_module_test_dynamic.lib \
    caffe2_nvrtc.lib \
    clog.lib \
    cpuinfo.lib \
    dnnl.lib \
    fbgemm.lib \
    #libprotobuf-lite.lib \
    #libprotobuf.lib \
    #libprotoc.lib \
    #mkldnn.lib \
    torch.lib \
    torch_cpu.lib \
    torch_cuda.lib
# to force linker use cuda version of the library
LIBS += -INCLUDE:?warp_size@cuda@at@@YAHXZ

# akvirtualcamera
INCLUDEPATH += $$PWD\..\lib\akvirtualcamera\VCamUtils\src
win32:CONFIG(release, debug|release): {
    LIBS += -L$${OUT_PWD}\..\lib\akvirtualcamera\VCamUtils\release\cl\x86_64\bin
    LIBS += -L$${OUT_PWD}\..\lib\akvirtualcamera\dshow\VCamIPC\release\cl\x86_64\bin\
}
else:win32:CONFIG(debug, debug|release): {
    LIBS += -L$${OUT_PWD}\..\lib\akvirtualcamera\VCamUtils\debug\cl\x86_64\bin
    LIBS += -L$${OUT_PWD}\..\lib\akvirtualcamera\dshow\VCamIPC\debug\cl\x86_64\bin\
}
LIBS += VCamUtils.lib VCamIPC.lib

# libyuv
include($$PWD/../lib/libyuv.pri)

# dlib
INCLUDEPATH += $$PWD/../lib/dlib
SOURCES += \
    $$PWD/../lib/dlib/dlib/all/source.cpp

# eigen
INCLUDEPATH += $$PWD/../lib/eigen

QML_IMPORT_NAME = io.qt.examples.backend
QML_IMPORT_MAJOR_VERSION = 1

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
