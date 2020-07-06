#ifndef IDENTITYPREDICTOR_H
#define IDENTITYPREDICTOR_H

#include "Predictor.h"

class IdentityPredictor : Predictor {
public:
    void setSourceImage(QString &avatarPath) override;

    QImage predict(QImage &drivingFrame) override;
};

#endif //IDENTITYPREDICTOR_H
