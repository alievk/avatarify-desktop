#include "KalmanCrop.h"

const QString KalmanCrop::shapePredictorWeightsPath =
        QDir::homePath() + "/.avatarify/models/shape_predictor_5_face_landmarks.dat";

KalmanCrop::KalmanCrop() : m_dlibPredictor(shapePredictorWeightsPath) {
}

QImage KalmanCrop::apply(const QImage &image) {
    std::vector<dlib::point> points = m_dlibPredictor.detect(image);
    if (points.empty()) {
        qDebug() << "no points";
        return image;
    }

    // first solution: find top, bottom, centerX, and then crop in 16x9
    int top = 720, bottom = 0, centerX = 0;
    for (auto point : points) {
        top = top > point.y() ? point.y() : top;
        bottom = bottom < point.y() ? point.y() : bottom;
        centerX += point.x();
    }
    centerX /= points.size();
    double scale = (float(bottom) - float(top)) / 9;
    top -= 19 * scale;
    bottom += 15 * scale;
    scale = (float(bottom) - float(top)) / 9;
    long left = long(centerX) - 8 * scale;

    QRect rect(left, top, long(16 * scale), bottom - top);
    QImage cropped = image.copy(rect);
    return cropped.scaled(1280, 720);
}


