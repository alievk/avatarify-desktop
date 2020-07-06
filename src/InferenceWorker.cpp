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
    libtorchPredictor.setSourceImage(m_avatarPath);
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
    QImage drivingFrame = m_camera->frame();
    QImage generatedFrame;
    if (m_avatarPath != "none") {
        generatedFrame = libtorchPredictor.predict(drivingFrame);
    } else {
        generatedFrame = identityPredictor.predict(drivingFrame);
    }

    // preview
    QImage generatedFrameRGBA = generatedFrame.convertToFormat(QImage::Format_ARGB32);
    QVideoFrame previewFrame(m_mirror ? generatedFrameRGBA.mirrored(true, false) : generatedFrameRGBA);
    m_videoSurface->present(previewFrame);

    // TODO: virtual camera
}
