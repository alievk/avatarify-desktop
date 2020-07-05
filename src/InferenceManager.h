#ifndef INFERENCEMANAGER_H
#define INFERENCEMANAGER_H

#include <QObject>
#include "AsyncCameraCapture.h"

class InferenceManager : public QObject {
Q_OBJECT
    Q_PROPERTY(AsyncCameraCapture* camera READ camera WRITE setCamera)

public:
    AsyncCameraCapture *camera();

    void setCamera(AsyncCameraCapture *camera);

private:
    AsyncCameraCapture *m_camera;
};


#endif //INFERENCEMANAGER_H
