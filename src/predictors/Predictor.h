#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <QImage>

class Predictor {
public:
    void setSourceImage(QImage &newAvatar);

    virtual QImage predict(QImage &frame) = 0;

private:
    QImage avatar;
};

#endif //PREDICTOR_H
