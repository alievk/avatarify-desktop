#include "AsyncCameraCapture.h"

template <typename T>
QImage paddedImage(const QImage & source, int padWidth, T padValue) {
    QImage padded{source.width() + 2*padWidth, source.height(), source.format()};
    padded.fill(padValue);
    QPainter p{&padded};
    p.drawImage(QPoint(padWidth, 0), source);
    return padded;
}

AsyncCameraCapture::AsyncCameraCapture(QObject *parent) : QObject(parent), outputResolution(1280, 720) {
    m_cameraInfo = QCameraInfo::defaultCamera();
    m_vfsettings.reset(new QCameraViewfinderSettings());
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

void AsyncCameraCapture::setCamera(const QCameraInfo &cameraInfo) {
    qDebug() << "cameraInfo: " << cameraInfo;
    if (cameraInfo.deviceName().startsWith("@device:sw:{")) {
        return;
    }

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
    QVideoFrame cloneFrame(frame);
    if (cloneFrame.isValid()) {
        if (cloneFrame.map(QAbstractVideoBuffer::ReadOnly)) {
            QVideoFrame::PixelFormat pixelFormat = cloneFrame.pixelFormat();
            QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(pixelFormat);

//            qDebug() << "AsyncCameraCapture::processFrame " << pixelFormat << " " << imageFormat;

            QImage image;
            if (imageFormat != QImage::Format_Invalid) {
                image = QImage(cloneFrame.bits(), cloneFrame.width(), cloneFrame.height(), imageFormat).mirrored().copy();
            } else if (pixelFormat == QVideoFrame::Format_NV12 ||
                       pixelFormat == QVideoFrame::Format_NV21 ||
                       pixelFormat == QVideoFrame::Format_YUYV ||
                       pixelFormat == QVideoFrame::Format_BGR24) {
                image = QImage(cloneFrame.width(), cloneFrame.height(), QImage::Format_BGR888);
                convertToRGB(cloneFrame.bits(), image.bits(), pixelFormat, cloneFrame.width(), cloneFrame.height());
            } else {
                qDebug() << "Not supported format: " << cloneFrame.pixelFormat();
                cloneFrame.unmap();
                return;
            }
            cloneFrame.unmap();

            if (imageFormat != QImage::Format_RGB888) {
                image = image.convertToFormat(QImage::Format_RGB888);
            }
            if (image.width() == 640 || image.height() == 480) {
                image = paddedImage(image, 107, QColor(Qt::black));
            }
            if (image.width() != 1280 || image.height() != 720) {
                image = image.scaled(1280, 720);
            }

            if (m_smartCropFlag) {
                image = m_smartCrop.apply(image);
            }

            Q_EMIT present(image);
        }
    }
}

bool AsyncCameraCapture::smartCrop() const {
    return m_smartCropFlag;
}

void AsyncCameraCapture::setSmartCrop(const bool smartCrop) {
    qDebug() << "AsyncCameraCapture::setSmartCrop " << QString::number(smartCrop);
    m_smartCrop.erase();
    m_smartCropFlag = smartCrop;
}

void AsyncCameraCapture::convertToRGB(uint8_t *src, uint8_t *dst, QVideoFrame::PixelFormat format,
                                      uint32_t width, uint32_t height) {
    static const uint8_t *pY, *pUV;
    static std::string buffer;
    if (buffer.size() != width * height * 4) {
        buffer.resize(width * height * 4);
    }
    auto *argbBuffer = (uint8_t *) &buffer[0];
    switch (format) {
        case QVideoFrame::Format_NV12:
            pY = src;
            pUV = src + width * height;
            libyuv::NV12ToRGB24(pY, width, pUV, width, dst, width * 3, width, height);
            break;
        case QVideoFrame::Format_NV21:
            pY = src;
            pUV = src + width * height;
            libyuv::NV21ToRGB24(pY, width, pUV, width, dst, width * 3, width, height);
            break;
        case QVideoFrame::Format_YUYV:  //  YUYV = YUY2
            libyuv::YUY2ToARGB(src, width * 2, argbBuffer, width * 4, width, height);
            libyuv::ARGBToRGB24(argbBuffer, width * 4, dst, width * 3, width, height);
            break;
        case QVideoFrame::Format_BGR24:
            for (uint32_t i = 0; i < width * height * 3; i += 3) {
                dst[i] = src[width * height * 3 - i - 3];
                dst[i + 1] = src[width * height * 3 - i - 2];
                dst[i + 2] = src[width * height * 3 - i - 1];
            }
            break;
    }
}
