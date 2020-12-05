#include "KalmanCrop.h"

#include "../../utils/facefinder.h"

const QString KalmanCrop::shapePredictorWeightsPath =
        QDir::homePath() + "/.avatarify/models/shape_predictor_5_face_landmarks.dat";

KalmanCrop::KalmanCrop() : m_dlibPredictor(shapePredictorWeightsPath), m_kalmanFilter(4, 20, 0.0001) {
    m_kalmanFilter.setInitialized(false);
}

void KalmanCrop::erase() {
    m_kalmanFilter.setInitialized(false);
}

QImage KalmanCrop::apply(const QImage &image) {
    //auto points = FaceFinder::findFace(image);
    //if (points.size() < 4) {
    //    if (m_points.size() < 4) {
    //        qDebug() << "KalmanCrop returns source image";
    //        return image;
    //    }
    //} else {
    //    m_points = points;
    //}
    //if (m_points.size() < 4) {
    //    qDebug() << "IMPOSSIBLE";
    //    return image;
    //}
    //auto left = image.width() * m_points[0];
    //auto top = image.height() * m_points[1];
    //auto right = image.width() * m_points[2];
    //auto bottom = image.height() * m_points[3];
    //auto avg = ((right - left) + (bottom - top)) / 2;
    //auto centerX = (left + right) / 2;
    //auto centerY = (top + bottom) / 2;
    //return image.copy(QRect(centerX - avg / 2, centerY - avg / 2, avg, avg));
    static double left, right, top, bottom, centerX, centerY, scale;

    auto points = FaceFinder::findFace(image.scaled(320, 240, Qt::KeepAspectRatio));//m_dlibPredictor.detect(image);
    std::vector<double> smoothedCropParams;
    if (points.size() >= 4) {
        // find top, bottom, centerX, and then crop in 16x9

        //left = image.width(), right = 0, top = image.height(), bottom = 0;
        left = points[0] * image.width();
        top = points[1] * image.height();
        right = points[2] * image.width();
        bottom = points[3] * image.height();
        //for (auto point : points) {
        //    left = left > point.x() ? point.x() : left;
        //    right = right < point.x() ? point.x() : right;
        //    top = top > point.y() ? point.y() : top;
        //    bottom = bottom < point.y() ? point.y() : bottom;
        //}
        //qDebug() << "before: " << top << "x" << right << "x" << bottom << "x" << left;

        std::vector<double> x = {left, right, top, bottom};
        if (!m_kalmanFilter.isInitialized()) {
            m_kalmanFilter.initFilters(x);
        }
        smoothedCropParams = m_kalmanFilter.step(x);
    } else {
        if (!m_kalmanFilter.isInitialized()) {
            qDebug() << "no face detected; not initialized; return source image";
            return image.copy(0, 0, qMin(image.width(), image.height()), qMin(image.width(), image.height()));
        }
        qDebug() << "no face detected; use last state";
        smoothedCropParams = m_kalmanFilter.state();
    }
    left = smoothedCropParams[0];
    right = smoothedCropParams[1];
    top = smoothedCropParams[2];
    bottom = smoothedCropParams[3];
//    qDebug() << "after: " << top << "x" << right << "x" << bottom << "x" << left;

    if (left != left || right != right || top != top || bottom != bottom) {
        qDebug() << "init!";
        std::vector<double> newX = {0, static_cast<double>(image.width()), 0, static_cast<double>(image.height())};
        m_kalmanFilter.initFilters(newX);
    }

    QRect rect;
    if (image.width() >= image.height()) {
        rect = QRect(long(left + right - image.height()) / 2, 0, image.height(), image.height());
    } else {
        rect = QRect(0, long(top + bottom - image.width()) / 2, image.width(), image.width());
    }

    //double scaleK = 1.666;
    //centerX = (left + right) / 2;
    //centerY = (top + bottom) / 2;
    //scale = ((bottom - top) + (right - left)) / 4.0 * scaleK;
    //left = centerX - scale;
    //top = centerY - scale;

    return image.copy(rect);
}


