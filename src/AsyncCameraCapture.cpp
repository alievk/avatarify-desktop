#include <iostream>
#include "AsyncCameraCapture.h"

AsyncCameraCapture::AsyncCameraCapture(QObject *parent) : QObject(parent) {
    m_cameraInfo = QCameraInfo::defaultCamera();

    m_vfsettings.reset(new QCameraViewfinderSettings());
    m_vfsettings->setResolution(640, 480);
    m_vfsettings->setMinimumFrameRate(15.0);
    m_vfsettings->setMaximumFrameRate(30.0);

    setCamera(m_cameraInfo);
}

QString AsyncCameraCapture::deviceId() {
    return m_cameraInfo.deviceName();
}

void AsyncCameraCapture::setDeviceId(const QString &deviceId) {
    qDebug() << "AsyncCameraCapture::setDeviceId " << deviceId;

    if (deviceId == m_cameraInfo.deviceName())
        return;

    for (const auto &cameraInfo : QCameraInfo::availableCameras()) {
        if (cameraInfo.deviceName() == deviceId) { // found
            m_cameraInfo = cameraInfo;
            setCamera(m_cameraInfo);
            return;
        }
    }
    qDebug() << "AsyncCameraCapture::setDeviceId: Camera with this name is not found!";
}

QImage AsyncCameraCapture::frame() {
    return m_frame;
}

void AsyncCameraCapture::setCamera(const QCameraInfo &cameraInfo) {
    m_camera.reset(new QCamera(cameraInfo));
    m_camera->setCaptureMode(QCamera::CaptureVideo);
    m_camera->setViewfinderSettings(*m_vfsettings);
    m_videoprobe.reset(new PoorMansProbe());
    connect(m_videoprobe.data(), &PoorMansProbe::videoFrameProbed, this, &AsyncCameraCapture::processFrame);
    m_videoprobe->setSource(m_camera.data());
    m_camera->start();
}

void AsyncCameraCapture::processFrame(const QVideoFrame &frame) {
//    qDebug() << "AsyncCameraCapture::processFrame";

    QVideoFrame cloneFrame(frame);
    cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
    const QImage image(cloneFrame.bits(), cloneFrame.width(), cloneFrame.height(),
                       QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));

    // preprocessing (mirror, crop/smartCrop) will be here

    m_frame = image;
}

bool AsyncCameraCapture::mirror() const {
    return m_mirror;
}

void AsyncCameraCapture::setMirror(const bool mirror) {
    qDebug() << "AsyncCameraCapture::setMirror " << QString::number(mirror);
    m_mirror = mirror;
}

bool AsyncCameraCapture::smartCrop() const {
    return m_smartCrop;
}

void AsyncCameraCapture::setSmartCrop(const bool smartCrop) {
    qDebug() << "AsyncCameraCapture::setSmartCrop " << QString::number(smartCrop);
    m_smartCrop = smartCrop;
}
