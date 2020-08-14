#ifndef KALMANCROP_H
#define KALMANCROP_H

#include <iostream>
#include <QtGui/QImage>
#include <QtCore/QDir>
#include "DlibShapePredictor.h"
#include "KalmanFilter1d.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

class KalmanCrop {
public:
    KalmanCrop();

    void erase();

    QImage apply(const QImage &image);

    static const QString shapePredictorWeightsPath;

private:
    DlibShapePredictor m_dlibPredictor;

    KalmanFilter1d m_kalmanFilter;
};


#endif //KALMANCROP_H
