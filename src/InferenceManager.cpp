#include <QVideoSurfaceFormat>
#include "InferenceManager.h"

InferenceManager::~InferenceManager() {
    if (worker != nullptr) {
        worker->stop();
        worker->wait();
    }
}

QString InferenceManager::rootFolder() {
//    qDebug() << ROOT_FOLDER;
    return ROOT_FOLDER;
}

AsyncCameraCapture *InferenceManager::camera() {
    return m_camera;
}

void InferenceManager::setCamera(AsyncCameraCapture *camera) {
    qDebug() << "InferenceManager::setCamera";
    m_camera = camera;
    startWorkerIfReady();
}

AbstractVCam *InferenceManager::virtualCamera() {
    return m_virtualCamera;
}

void InferenceManager::setVirtualCamera(AbstractVCam *virtualCamera) {
    qDebug() << "InferenceManager::setVirtualCamera";
    if (m_virtualCamera == virtualCamera)
        return;
    m_virtualCamera = virtualCamera;
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
}

QString InferenceManager::avatarPath() const {
    return m_avatarPath;
}

void InferenceManager::setAvatarPath(const QString &avatarFilename) {
    qDebug() << "InferenceManager::setAvatarPath " << avatarFilename;

    QString avatarPath = ROOT_FOLDER + "/.avatarify/avatars/" + avatarFilename;
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

            QVideoSurfaceFormat format(QSize(640, 480), QVideoFrame::PixelFormat::Format_ARGB32);
            m_videoSurface->start(format);

            worker.reset(new InferenceWorker(m_camera));
            connect(worker.data(), &InferenceWorker::presentPreview, this, &InferenceManager::presentPreviewFrame);
            connect(worker.data(), &QThread::finished, worker.data(), &QObject::deleteLater);
            worker->start();
        } else {
            qDebug() << "Refuse to start worker, worker is already started.";
        }
    } else {
        qDebug() << "Refuse to start worker, not all params are filled.";
    }
}

void InferenceManager::presentPreviewFrame(const QImage &generatedFrame) {
    QImage generatedFrameRGBA = generatedFrame.convertToFormat(QImage::Format_ARGB32);
    QVideoFrame previewFrame(m_mirror ? generatedFrameRGBA.mirrored(true, false) : generatedFrameRGBA);
    m_videoSurface->present(previewFrame);
}
