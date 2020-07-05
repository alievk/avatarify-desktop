QT += quick \
    quickcontrols2 \
    multimedia

CONFIG += c++14 \
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
    src/AsyncCameraCapture.h \
    src/PoorMansProbe.h \
    src/predictors/Predictor.h \
    src/predictors/IdentityPredictor.h \
    src/InferenceManager.h \
    src/InferenceWorker.h

SOURCES += \
    src/AsyncCameraCapture.cpp \
    src/PoorMansProbe.cpp \
    src/predictors/Predictor.cpp \
    src/predictors/IdentityPredictor.cpp \
    src/InferenceManager.cpp \
    src/InferenceWorker.cpp \
    src/main.cpp

RESOURCES += src/qml.qrc

OTHER_FILES = src/main.qml \
    src/qtquickcontrols2.conf

TRANSLATIONS += \
    src/avatarify-desktop_en_US.ts

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
