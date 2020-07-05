#include "InferenceManager.h"

AsyncCameraCapture *InferenceManager::camera() {
    return m_camera;
}

void InferenceManager::setCamera(AsyncCameraCapture *camera) {
    qDebug() << "InferenceManager::setCamera";
    m_camera = camera;
}
