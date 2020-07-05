#ifndef INFERENCEWORKER_H
#define INFERENCEWORKER_H

#include <QThread>
#include <QAbstractVideoSurface>
#include <src/predictors/IdentityPredictor.h>
#include "AsyncCameraCapture.h"

class InferenceWorker : public QThread {
Q_OBJECT

public:
    InferenceWorker(AsyncCameraCapture *camera, QAbstractVideoSurface *videoSurface, bool virtualCamera);

public slots:
    void setMirror(bool mirror);

    void setVirtualCamera(bool virtualCamera);

    void stop();

    void run() override;

private:
    void inference();

    const int fpsLimit = 25;

    AsyncCameraCapture *m_camera;
    QAbstractVideoSurface *m_videoSurface;
    IdentityPredictor predictor;
    bool m_mirror{};
    bool m_virtualCamera;
    bool isAlive = true;
};


#endif //INFERENCEWORKER_H
