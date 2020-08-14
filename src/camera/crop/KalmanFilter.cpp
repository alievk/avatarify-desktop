#include "KalmanFilter.h"

typedef bool b;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

bool KalmanFilter::isInitialized() const {
    return m_initialized;
}

void KalmanFilter::setInitialized(bool initialized) {
    m_initialized = initialized;
}

void KalmanFilter::init(VectorXd &x0, MatrixXd &P, MatrixXd &F, MatrixXd &H, MatrixXd &R, MatrixXd &Q) {
    m_x = std::move(x0);
    m_P = std::move(P);
    m_F = std::move(F);
    m_H = std::move(H);
    m_R = std::move(R);
    m_Q = std::move(Q);

    m_initialized = true;
}

VectorXd &KalmanFilter::state() {
    return m_x;
}

void KalmanFilter::predict() {
    //Use the state using the state transition matrix
    m_x = m_F * m_x;
    //Update the covariance matrix using the process noise and state transition matrix
    MatrixXd Ft = m_F.transpose();
    m_P = m_F * m_P * Ft + m_Q;
}

void KalmanFilter::update(const VectorXd &z) {
    MatrixXd Ht = m_H.transpose();
    MatrixXd PHt = m_P * Ht;

    VectorXd y = z - m_H * m_x;
    MatrixXd S = m_H * PHt + m_R;
    MatrixXd K = PHt * S.inverse();

    //update State
    m_x = m_x + (K * y);
    //update covariance matrix
    long x_size = m_x.size();
    MatrixXd I = MatrixXd::Identity(x_size, x_size);
    m_P = (I - K * m_H) * m_P;
}

VectorXd &KalmanFilter::step(const VectorXd &z) {
    predict();
    update(z);
    return m_x;
}