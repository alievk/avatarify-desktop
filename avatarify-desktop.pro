QT += quick \
    quickcontrols2 \
    multimedia

CONFIG += c++14 \
    qmltypes

SUBDIRS = "$$PWD/lib/akvirtualcamera"

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
    src/camera/yuv2rgb.h \
    src/predictors/IdentityPredictor.h \
    src/predictors/LibtorchFOMM.h \
    src/predictors/LibtorchIdentityPredictor.h \
    src/predictors/LibtorchPredictor.h \
    src/predictors/Predictor.h \
    src/vcam/AbstractVCamInterface.h \
    src/vcam/AkVCamBridge.h \
    src/vcam/StubVCam.h \
    src/InferenceManager.h \
    src/InferenceWorker.h \

SOURCES += \
    src/camera/AsyncCameraCapture.cpp \
    src/camera/PoorMansProbe.cpp \
    src/camera/yuv2rgb.cpp \
    src/predictors/IdentityPredictor.cpp \
    src/predictors/LibtorchFOMM.cpp \
    src/predictors/LibtorchIdentityPredictor.cpp \
    src/predictors/LibtorchPredictor.cpp \
    src/predictors/Predictor.cpp \
    src/vcam/AbstractVCamInterface.cpp \
    src/vcam/AkVCamBridge.cpp \
    src/vcam/StubVCam.cpp \
    src/InferenceManager.cpp \
    src/InferenceWorker.cpp \
    src/main.cpp \

RESOURCES += src/qml.qrc

OTHER_FILES = src/main.qml \
    src/qtquickcontrols2.conf

TRANSLATIONS += \
    src/avatarify-desktop_en_US.ts

QMAKE_CXXFLAGS += -DGLIBCXX_USE_CXX11_ABI=0
CONFIG += no_keywords

INCLUDEPATH += "$$PWD\lib\libtorch\include"
INCLUDEPATH += "$$PWD\lib\libtorch\include\torch\csrc\api\include"

LIBS += -L$$PWD/lib/libtorch/lib
LIBS += -ltorch -lc10

LIBS += -L$${OUT_PWD}/../../VCamUtils/$${BIN_DIR} -lVCamUtils \

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

QML_IMPORT_NAME = io.qt.examples.backend
QML_IMPORT_MAJOR_VERSION = 1

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/.gitignore
