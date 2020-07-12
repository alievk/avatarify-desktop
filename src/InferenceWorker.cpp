#include <QTime>
#include <qtestsupport_core.h>
#include <QVideoSurfaceFormat>
#include <utility>
#include "InferenceWorker.h"

InferenceWorker::InferenceWorker(AsyncCameraCapture *camera) {
    m_camera = camera;
}

void InferenceWorker::setAvatarPath(QString avatarPath) {
    m_avatarPath = std::move(avatarPath);
    if (m_avatarPath != "none") {
        libtorchPredictor.setSourceImage(m_avatarPath);
    }
}

void InferenceWorker::stop() {
    isAlive = false;
}

void InferenceWorker::run() {
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

    presentPreview(generatedFrame);
    // TODO: virtual camera
}
