#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QCamera>
#include <QVideoProbe>
#include <QImage>
#include <QCameraInfo>
#include "PoorMansProbe.h"
#include "yuv2rgb.h"
#include "vpImageConvert.h"
#include <iostream>

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

    QImage frame();

private Q_SLOTS:

    void setCamera(const QCameraInfo &cameraInfo);

    void processFrame(const QVideoFrame &frame);

private:
    QScopedPointer<QCamera> m_camera;
    QScopedPointer<PoorMansProbe> m_videoprobe;
    QScopedPointer<QCameraViewfinderSettings> m_vfsettings;

    const QSize outputResolution;
    QImage m_frame;


    QCameraInfo m_cameraInfo;
    bool m_smartCrop = false;

};

#endif // BACKEND_H
