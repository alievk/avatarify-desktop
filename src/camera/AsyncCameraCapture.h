#ifndef BACKEND_H
#define BACKEND_H

#include <iostream>
#include <QObject>
#include <QString>
#include <QCamera>
#include <QVideoProbe>
#include <QImage>
#include <QCameraInfo>
#include <QPainter>
#include <libyuv/convert_argb.h>
#include <libyuv/convert_from_argb.h>
#include "PoorMansProbe.h"
#include "crop/KalmanCrop.h"

class AsyncCameraCapture : public QObject {
Q_OBJECT
    Q_PROPERTY(QString deviceId READ deviceId WRITE setDeviceId)
    Q_PROPERTY(bool smartCrop READ smartCrop WRITE setSmartCrop)

public:
    explicit AsyncCameraCapture(QObject *parent = nullptr);

    QString deviceId();

    void setDeviceId(const QString &deviceId);

    bool smartCrop() const;

    void setSmartCrop(bool smartCrop);

Q_SIGNALS:

    void present(QImage &frame);

private Q_SLOTS:

    void setCamera(const QCameraInfo &cameraInfo);

    void processFrame(const QVideoFrame &frame);

private:
    const QSize outputResolution;

    static void convertToRGB(uint8_t *src, uint8_t *dest, QVideoFrame::PixelFormat format,
                             uint32_t width, uint32_t height);

    QScopedPointer<QCamera> m_camera;
    QScopedPointer<PoorMansProbe> m_videoprobe;
    QScopedPointer<QCameraViewfinderSettings> m_vfsettings;
    QCameraInfo m_cameraInfo;

    bool m_smartCropFlag = false;
    KalmanCrop m_smartCrop;
};

#endif // BACKEND_H
