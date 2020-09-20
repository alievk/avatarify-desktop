#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <QImage>
#include <QString>

class Predictor {
public:
    virtual void setSourceImage(QString &avatarPath) = 0;

    virtual QImage predict(QImage &frame) = 0;
};

#endif //PREDICTOR_H
