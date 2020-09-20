#include "IdentityPredictor.h"
#include <QDebug>

void IdentityPredictor::setSourceImage(QString &avatarPath) {
    qDebug() << "IdentityPredictor::setSourceImage";
}


QImage IdentityPredictor::predict(QImage &drivingFrame) {
//    qDebug() << "IdentityPredictor::predict";
    return drivingFrame;
}
