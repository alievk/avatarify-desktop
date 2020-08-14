#ifndef KALMANCROP_H
#define KALMANCROP_H

#include <iostream>
#include <QtGui/QImage>
#include <QtCore/QDir>
#include "DlibShapePredictor.h"
#include "KalmanFilter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

class KalmanCrop {
public:
    KalmanCrop();

    void erase();

    QImage apply(const QImage &image);

    static const QString shapePredictorWeightsPath;

private:
    void initKalman(VectorXd &x0);

    DlibShapePredictor m_dlibPredictor;

    KalmanFilter m_kalmanFilter;
};


#endif //KALMANCROP_H
