#ifndef IDENTITYPREDICTOR_H
#define IDENTITYPREDICTOR_H

#include "Predictor.h"

class IdentityPredictor : Predictor {
public:
    QImage predict(QImage &frame) override;
};

#endif //IDENTITYPREDICTOR_H
