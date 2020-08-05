#include <QTime>
#include <qtestsupport_core.h>
#include <QVideoSurfaceFormat>
#include <utility>
#include "InferenceWorker.h"

InferenceWorker::InferenceWorker(AsyncCameraCapture *camera) : m_camera(camera),
                                                               m_frame(1280, 720, QImage::Format_RGB888) {
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

void InferenceWorker::setAvatarPath(QString avatarPath) {
    m_avatarPath = std::move(avatarPath);
    if (m_avatarPath != "none") {
        m_fommPredictor.setSourceImage(m_avatarPath);
    }
}

void InferenceWorker::setFrame(QImage &frame) {
    std::copy(frame.bits(), frame.bits() + frame.sizeInBytes(), m_frame.bits());
}

void InferenceWorker::stop() {
    isAlive = false;
}

void InferenceWorker::inference() {
    QImage generatedFrame;
    if (m_avatarPath != "none") {
        generatedFrame = m_fommPredictor.predict(m_frame);
    } else {
        generatedFrame = m_identityPredictor.predict(m_frame);
    }
    if (generatedFrame.width() > 0) {
        present(generatedFrame);
    }
}
