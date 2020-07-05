#include "InferenceManager.h"

InferenceManager::~InferenceManager() {
    if (worker != nullptr) {
        worker->stop();
        worker->quit();
        worker->wait();
    }
}

AsyncCameraCapture *InferenceManager::camera() {
    return m_camera;
}

void InferenceManager::setCamera(AsyncCameraCapture *camera) {
    qDebug() << "InferenceManager::setCamera";
    m_camera = camera;
    startWorkerIfReady();
}

QAbstractVideoSurface *InferenceManager::videoSurface() {
    return m_videoSurface;
}

void InferenceManager::setVideoSurface(QAbstractVideoSurface *videoSurface) {
    qDebug() << "InferenceManager::setVideoSurface";
    m_videoSurface = videoSurface;
    startWorkerIfReady();
}

bool InferenceManager::mirror() const {
    return m_mirror;
}

void InferenceManager::setMirror(bool mirror) {
    qDebug() << "InferenceManager::setMirror" << mirror;

    if (m_mirror == mirror)
        return;

    m_mirror = mirror;
    if (worker != nullptr) {
        worker->setMirror(m_mirror);
    }
}

bool InferenceManager::virtualCamera() const {
    return m_virtualCamera;
}

void InferenceManager::setVirtualCamera(bool virtualCamera) {
    qDebug() << "InferenceManager::setVirtualCamera" << virtualCamera;

    if (m_virtualCamera == virtualCamera)
        return;

    m_virtualCamera = virtualCamera;
    if (worker != nullptr) {
        worker->setVirtualCamera(m_virtualCamera);
    }
}

QString InferenceManager::avatarPath() const {
    return m_avatarPath;
}

void InferenceManager::setAvatarPath(QString avatarPath) {
    qDebug() << "InferenceManager::setAvatarPath " << avatarPath;

    if (m_avatarPath == avatarPath)
        return;

    m_avatarPath = avatarPath;
    if (worker != nullptr) {
        worker->setAvatarPath(m_avatarPath);
    }
}

void InferenceManager::startWorkerIfReady() {
    if (m_camera != nullptr && m_videoSurface != nullptr) {
        if (worker == nullptr) {
            qDebug() << "Start worker!";
            worker.reset(new InferenceWorker(m_camera, m_videoSurface, m_virtualCamera));
            connect(worker.data(), &QThread::finished, worker.data(), &QObject::deleteLater);
            worker->start();
        } else {
            qDebug() << "Refuse to start worker, worker is already started.";
        }
    } else {
        qDebug() << "Refuse to start worker, not all params are filled.";
    }
}

