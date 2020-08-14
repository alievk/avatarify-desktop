#ifndef KALMANCROP_H
#define KALMANCROP_H

#include <iostream>
#include <QtGui/QImage>
#include <QtCore/QDir>
#include "DlibShapePredictor.h"


class KalmanCrop {
public:
    KalmanCrop();

    QImage apply(const QImage &image);

    static const QString shapePredictorWeightsPath;

private:

    DlibShapePredictor m_dlibPredictor;
};


#endif //KALMANCROP_H
