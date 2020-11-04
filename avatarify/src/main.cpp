#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QQmlContext>
#include "camera/AsyncCameraCapture.h"
#include "imagecropper/ImageCropperWidget.h"
#include "InferenceManager.h"

#include "utils/amplitudelogger.h"
#include "utils/facefinder.h"

#if defined(WIN32)

#include "vcam/AkVCamBridge.h"

typedef AkVCamBridge VCamImpl;
#else

#include "vcam/StubVCam.h"

typedef StubVCam VCamImpl;
#endif

auto constexpr AMPLITUDE_API_KEY = "19064b86fc8ccf247265200e442a6aad";

int main(int argc, char *argv[]) {
    AmplitudeLogger::setApiKey(AMPLITUDE_API_KEY);
    AmplitudeLogger::log("launch");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Avatarify");
    app.setOrganizationDomain("avatarify.com");
    app.setApplicationName("Avatarify Desktop");
    app.setWindowIcon(QIcon(":/resources/favicon"));

    qmlRegisterType<AsyncCameraCapture>("com.avatarify.desktop", 1, 0, "AsyncCameraCapture");
    qmlRegisterType<InferenceManager>("com.avatarify.desktop", 1, 0, "InferenceManager");
    qmlRegisterType<VCamImpl>("com.avatarify.desktop", 1, 0, "VCam");
//    qmlRegisterType<ImageCropperWidget>("com.avatarify.desktop", 1, 0, "ImageCropper");

    FaceFinder faceFinder;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("faceFinder", &faceFinder);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    qDebug() << "Is CUDA available????? " << torch::cuda::is_available();

    return app.exec();
}
