#include "KalmanCrop.h"

const QString KalmanCrop::shapePredictorWeightsPath =
        QDir::homePath() + "/.avatarify/models/shape_predictor_5_face_landmarks.dat";

KalmanCrop::KalmanCrop() : m_dlibPredictor(shapePredictorWeightsPath), m_kalmanFilter(4, 20, 0.0001) {
    m_kalmanFilter.setInitialized(false);
}

void KalmanCrop::erase() {
    m_kalmanFilter.setInitialized(false);
}

QImage KalmanCrop::apply(const QImage &image) {
    static double left, right, top, bottom, centerX, centerY, scale;

    std::vector<dlib::point> points = m_dlibPredictor.detect(image);
    std::vector<double> smoothedCropParams;
    if (!points.empty()) {
        // find top, bottom, centerX, and then crop in 16x9
        left = 1280, right = 0, top = 720, bottom = 0;
        for (auto point : points) {
            left = left > point.x() ? point.x() : left;
            right = right < point.x() ? point.x() : right;
            top = top > point.y() ? point.y() : top;
            bottom = bottom < point.y() ? point.y() : bottom;
        }
//        qDebug() << "before: " << top << "x" << right << "x" << bottom << "x" << left;

        std::vector<double> x = {left, right, top, bottom};
        if (!m_kalmanFilter.isInitialized()) {
            m_kalmanFilter.initFilters(x);
        }
        smoothedCropParams = m_kalmanFilter.step(x);
    } else {
        if (!m_kalmanFilter.isInitialized()) {
            qDebug() << "no face detected; not initialized; return source image";
            return image;
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
        std::vector<double> newX = {0, 1280, 0, 720};
        m_kalmanFilter.initFilters(newX);
    }

    centerX = (left + right) / 2;
    centerY = (top + bottom) / 2;
    scale = (bottom - top) / 1.5;
    left = centerX - 8 * scale;
    top = centerY - 4.5 * scale;

    QRect rect(long(left), long(top), long(16 * scale), long(9 * scale));
    return image.copy(rect).scaled(1280, 720);
}


