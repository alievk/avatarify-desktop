#include "KalmanCrop.h"

const QString KalmanCrop::shapePredictorWeightsPath =
        QDir::homePath() + "/.avatarify/models/shape_predictor_5_face_landmarks.dat";

KalmanCrop::KalmanCrop() : m_dlibPredictor(shapePredictorWeightsPath) {
    m_kalmanFilter.setInitialized(false);
}

void KalmanCrop::erase() {
    m_kalmanFilter.setInitialized(false);
}

void KalmanCrop::initKalman(VectorXd &x0) {
    int n = x0.size(); // Number of states
    int m = 1; // Number of measurements

    MatrixXd P(n, n);
    MatrixXd F(n, n);
    MatrixXd H(n, m);
    MatrixXd R(m, m);
    MatrixXd Q(m, m);

    P.setIdentity(m, m);
    F.setIdentity(m, m);
    H.setIdentity(m, n);
    R(0, 0) = 0.2;
    Q(0, 0) = 0.000001;

    m_kalmanFilter.init(x0, P, F, H, R, Q);
}


QImage KalmanCrop::apply(const QImage &image) {
    static long top, bottom, left;
    static double centerX, centerY, scale;

    std::vector<dlib::point> points = m_dlibPredictor.detect(image);
    VectorXd smoothedPoints;
    if (!points.empty()) {
        std::vector<double> pointsFlat;
        for (auto point : points) {
            pointsFlat.emplace_back(point.x());
            pointsFlat.emplace_back(point.y());
        }
        VectorXd x = Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(pointsFlat.data(), pointsFlat.size());
        if (!m_kalmanFilter.isInitialized()) {
            initKalman(x);
        }
        smoothedPoints = m_kalmanFilter.step(x);
    } else {
        if (!m_kalmanFilter.isInitialized()) {
            return image;
        }
        qDebug() << "no face detected; use last state";
        smoothedPoints = m_kalmanFilter.state();
    }
    for (int i = 0; i < smoothedPoints.size() / 2; ++i) {
        points[i] = dlib::point(smoothedPoints[2 * i], smoothedPoints[2 * i + 1]);
    }

    // find top, bottom, centerX, and then crop in 16x9
    top = 720, bottom = 0, centerX = 0, centerY = 0;
    for (auto point : points) {
        top = top > point.y() ? point.y() : top;
        bottom = bottom < point.y() ? point.y() : bottom;
        centerX += point.x();
        centerY += point.y();
    }
    centerX /= points.size();
    centerY /= points.size();
    scale = (float(bottom) - float(top)) / 2;

    left = centerX - 8 * scale;
    top = centerY - 4.5 * scale;

    QRect rect(left, top, long(16 * scale), long(9 * scale));
    return image.copy(rect).scaled(1280, 720);
}


