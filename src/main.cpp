#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "camera/AsyncCameraCapture.h"
#include "InferenceManager.h"

#if defined(WINDOWS)
#include "vcam/DirectShowVCam.h"
typedef DirectShowVCam VCamImpl;
#else

#include "vcam/StubVCam.h"

typedef StubVCam VCamImpl;
#endif

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Avatarify");
    app.setOrganizationDomain("avatarify.com");
    app.setApplicationName("Avatarify Desktop");

    qmlRegisterType<AsyncCameraCapture>("com.avatarify.desktop", 1, 0, "AsyncCameraCapture");
    qmlRegisterType<InferenceManager>("com.avatarify.desktop", 1, 0, "InferenceManager");
    qmlRegisterType<VCamImpl>("com.avatarify.desktop", 1, 0, "AbstractVCam");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
