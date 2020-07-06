#ifndef INFERENCEWORKER_H
#define INFERENCEWORKER_H

#include <QThread>
#include <QAbstractVideoSurface>
#include "AsyncCameraCapture.h"
#include "predictors/IdentityPredictor.h"
#include "predictors/LibtorchPredictor.h"

class InferenceWorker : public QThread {
Q_OBJECT

public:
    InferenceWorker(AsyncCameraCapture *camera, QAbstractVideoSurface *videoSurface, bool virtualCamera);

public Q_SLOTS:

    void setMirror(bool mirror);

    void setVirtualCamera(bool virtualCamera);

    void setAvatarPath(QString avatarPath);

    void stop();

    void run() override;

private:
    void inference();

    const int fpsLimit = 25;

    AsyncCameraCapture *m_camera;
    QAbstractVideoSurface *m_videoSurface;
    IdentityPredictor identityPredictor;
    LibtorchPredictor libtorchPredictor;
    bool m_mirror = false;
    bool m_virtualCamera = false;
    QString m_avatarPath = "none";
    bool isAlive = true;
};


#endif //INFERENCEWORKER_H
