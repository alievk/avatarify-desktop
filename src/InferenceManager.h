#ifndef INFERENCEMANAGER_H
#define INFERENCEMANAGER_H

#include <QObject>
#include <QThread>
#include "src/camera/AsyncCameraCapture.h"
#include "InferenceWorker.h"

class InferenceManager : public QObject {
Q_OBJECT
    Q_PROPERTY(AsyncCameraCapture *camera READ camera WRITE setCamera)
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface)
    Q_PROPERTY(bool mirror READ mirror WRITE setMirror)
    Q_PROPERTY(bool virtualCamera READ virtualCamera WRITE setVirtualCamera)
    Q_PROPERTY(QString avatarPath READ avatarPath WRITE setAvatarPath)

public:
    ~InferenceManager() override;

    AsyncCameraCapture *camera();

    void setCamera(AsyncCameraCapture *camera);

    QAbstractVideoSurface *videoSurface();

    void setVideoSurface(QAbstractVideoSurface *videoSurface);

    bool mirror() const;

    void setMirror(bool mirror);

    bool virtualCamera() const;

    void setVirtualCamera(bool virtualCamera);

    QString avatarPath() const;

    void setAvatarPath(const QString& avatarPath);

    void startWorkerIfReady();

public Q_SLOTS:
    void presentPreviewFrame(const QImage& generatedFrame);

private:
    AsyncCameraCapture *m_camera = nullptr;
    QAbstractVideoSurface *m_videoSurface = nullptr;
    bool m_mirror = false;
    bool m_virtualCamera = false;
    QString m_avatarPath;
    QScopedPointer<InferenceWorker> worker;
};


#endif //INFERENCEMANAGER_H
