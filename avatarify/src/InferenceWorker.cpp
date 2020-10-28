#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <qtestsupport_core.h>
#include <QVideoSurfaceFormat>
#include <utility>
#include "InferenceWorker.h"

InferenceWorker::InferenceWorker(AsyncCameraCapture *camera) : m_camera(camera),
                                                               m_frame(1280, 720, QImage::Format_RGB888) {
}

void InferenceWorker::run() {
    while (isAlive) {
        inference();
    }
}

void InferenceWorker::setAvatarPath(QString avatarPath) {
    m_avatarPath = std::move(avatarPath);
    if (m_avatarPath != "none") {
        m_fommPredictor.setSourceImage(m_avatarPath);
    }
}

void InferenceWorker::setFrame(QImage &frame) {
    if (!m_frameMutex.tryLock()) {
        return;
    }
    m_frame = std::move(frame);
    m_frameReady.wakeAll();
    m_frameMutex.unlock();
}

void InferenceWorker::stop() {
    isAlive = false;
    QtConcurrent::run([this]() {
        m_frameMutex.lock();
        m_frameReady.wakeAll();
        m_frameMutex.unlock();
    });
}

void InferenceWorker::inference() {
    m_frameMutex.lock();
    m_frameReady.wait(&m_frameMutex);
    if (!isAlive) {
        m_frameMutex.unlock();
        return;
    }
    QImage generatedFrame;
    if (m_avatarPath != "none") {
        generatedFrame = m_fommPredictor.predict(m_frame);
    } else {
        generatedFrame = m_identityPredictor.predict(m_frame);
    }
    m_frameMutex.unlock();
    if (generatedFrame.width() > 0) {
        present(generatedFrame);
    }
}
