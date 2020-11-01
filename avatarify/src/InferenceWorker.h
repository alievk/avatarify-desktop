#ifndef INFERENCEWORKER_H
#define INFERENCEWORKER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "camera/AsyncCameraCapture.h"
#include "predictors/IdentityPredictor.h"
#include "predictors/LibtorchIdentityPredictor.h"
#include "predictors/LibtorchFOMM.h"

class InferenceWorker : public QThread {
Q_OBJECT

public:
    explicit InferenceWorker(AsyncCameraCapture *camera);

    void run() override;

    void setAvatarPath(QString avatarPath);

    void requestCalibration();

Q_SIGNALS:

    void present(QImage generatedFrame);

public Q_SLOTS:

    void setFrame(QImage &frame);

    void stop();

private:
    void inference();

    AsyncCameraCapture *m_camera;
    IdentityPredictor m_identityPredictor;
    LibtorchFOMM m_fommPredictor;

    QString m_avatarPath = "none";
    QImage m_frame;
    bool isAlive = true;

    QMutex m_frameMutex;
    QWaitCondition m_frameReady;

    bool m_calibrationRequested{false};
};


#endif //INFERENCEWORKER_H
