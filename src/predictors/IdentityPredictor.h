#ifndef IDENTITYPREDICTOR_H
#define IDENTITYPREDICTOR_H

#include "Predictor.h"

class IdentityPredictor : Predictor {
public:
    void setSourceImage(QString &AvatarPath) override;

    QImage predict(QImage &frame) override;
};

#endif //IDENTITYPREDICTOR_H
