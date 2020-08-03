#include "AsyncCameraCapture.h"

AsyncCameraCapture::AsyncCameraCapture(QObject *parent) : QObject(parent), outputResolution(1280, 720),
                                                          m_frame(outputResolution, QImage::Format_RGB888) {
    m_cameraInfo = QCameraInfo::defaultCamera();

    m_vfsettings.reset(new QCameraViewfinderSettings());
//    m_vfsettings->setPixelAspectRatio(16, 9);
//    m_vfsettings->setMinimumFrameRate(15.0);
//    m_vfsettings->setMaximumFrameRate(30.0);

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
    qDebug() << "cameraInfo: " << cameraInfo;

    m_camera.reset(new QCamera(cameraInfo));
    m_camera->setCaptureMode(QCamera::CaptureViewfinder);
    m_videoprobe.reset(new PoorMansProbe());
    connect(m_videoprobe.data(), &PoorMansProbe::videoFrameProbed, this, &AsyncCameraCapture::processFrame);
    m_videoprobe->setSource(m_camera.data());
    m_camera->load();

    m_vfsettings.reset(new QCameraViewfinderSettings());
    qDebug() << m_camera->status();
    qDebug() << m_camera->supportedViewfinderResolutions(*m_vfsettings.data());
    m_vfsettings->setResolution(QSize(640, 480));
    qDebug() << m_camera->supportedViewfinderPixelFormats(*m_vfsettings.data());
//    m_vfsettings->setPixelFormat(QVideoFrame::Format_NV12);
    m_camera->setViewfinderSettings(*m_vfsettings);

    m_camera->start();
}

void AsyncCameraCapture::processFrame(const QVideoFrame &frame) {
//    qDebug() << "AsyncCameraCapture::processFrame";

    QVideoFrame cloneFrame(frame);
    cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
    QImage::Format format = QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat());
    QImage image;
    if (format != QImage::Format_Invalid) {
        image = QImage(cloneFrame.bits(), cloneFrame.width(), cloneFrame.height(), format);
    } else if (cloneFrame.pixelFormat() == QVideoFrame::Format_NV12) {
        image = QImage(cloneFrame.width(), cloneFrame.height(), QImage::Format_RGB888);
        nv12_to_rgb(image.bits(), cloneFrame.bits(), cloneFrame.width(), cloneFrame.height());
    } else if (cloneFrame.pixelFormat() == QVideoFrame::Format_NV21) {
        image = QImage(cloneFrame.width(), cloneFrame.height(), QImage::Format_RGB888);
        nv21_to_rgb(image.bits(), cloneFrame.bits(), cloneFrame.width(), cloneFrame.height());
    } else if (cloneFrame.pixelFormat() == QVideoFrame::Format_YUYV) {
        image = QImage(cloneFrame.width(), cloneFrame.height(), QImage::Format_RGB888);
        YUYVToRGB(cloneFrame.bits(), image.bits(), cloneFrame.width(), cloneFrame.height());
    } else {
        qDebug() << "FUCK " << cloneFrame.pixelFormat();
        int nbytes = cloneFrame.mappedBytes();
        image = QImage::fromData(cloneFrame.bits(), nbytes);
        qDebug() << image.width() << " " << image.height();
    }
    cloneFrame.unmap();

    if (format != QImage::Format_RGB888) {
        image = image.convertToFormat(QImage::Format_RGB888);
    }

    // TODO: crop/smartCrop

    m_frame = image;
}

bool AsyncCameraCapture::smartCrop() const {
    return m_smartCrop;
}

void AsyncCameraCapture::setSmartCrop(const bool smartCrop) {
    qDebug() << "AsyncCameraCapture::setSmartCrop " << QString::number(smartCrop);
    m_smartCrop = smartCrop;
}