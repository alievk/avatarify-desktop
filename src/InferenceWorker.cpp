#include <QTime>
#include <qtestsupport_core.h>
#include <QVideoSurfaceFormat>
#include <utility>
#include "InferenceWorker.h"

InferenceWorker::InferenceWorker(AsyncCameraCapture *camera, QAbstractVideoSurface *videoSurface, bool virtualCamera) {
    m_camera = camera;
    m_videoSurface = videoSurface;
    m_virtualCamera = virtualCamera;
}

void InferenceWorker::setMirror(bool mirror) {
    m_mirror = mirror;
}

void InferenceWorker::setVirtualCamera(bool virtualCamera) {
    m_virtualCamera = virtualCamera;
}

void InferenceWorker::setAvatarPath(QString avatarPath) {
    m_avatarPath = std::move(avatarPath);
}

void InferenceWorker::stop() {
    isAlive = false;
}

void InferenceWorker::run() {
    QVideoSurfaceFormat format(QSize(640, 480), QVideoFrame::PixelFormat::Format_ARGB32);
    m_videoSurface->start(format);

    qint64 targetTime = 1000 / fpsLimit;
    while (isAlive) {
        QTime startTime = QTime::currentTime();
        inference();
        qint64 sleepTimeMsec = targetTime - QTime::currentTime().msecsTo(startTime);
        if (sleepTimeMsec > 0) {
            QTest::qWait(sleepTimeMsec);
        }
    }
}

void InferenceWorker::inference() {
    QImage srcImage = m_camera->frame();
    QImage resultImage;
//    if (m_avatarPath != "none") {
//        resultImage = libtorchPredictor.predict(srcImage);
//    } else {
        resultImage = identityPredictor.predict(srcImage);
//    }

    // preview
    QVideoFrame previewFrame(m_mirror ? resultImage.mirrored(true, false) : resultImage);
    m_videoSurface->present(previewFrame);

    // TODO: virtual camera
}
