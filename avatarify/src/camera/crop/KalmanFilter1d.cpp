//
// Created by vlivashkin on 14.08.2020.
//

#include "KalmanFilter1d.h"

KalmanFilter1d::KalmanFilter1d(int ndim, double R, double Q) : ndim(ndim), m_R(R), m_Q(Q) {
    for (int i = 0; i < ndim; ++i) {
        filters.emplace_back(KalmanFilter());
    }
}

bool KalmanFilter1d::isInitialized() const {
    return m_initialized;
}

void KalmanFilter1d::setInitialized(bool initialized) {
    m_initialized = initialized;
}


void KalmanFilter1d::initFilters(std::vector<double> &x0) {
    qDebug() << "KalmanFilter1d::initFilters";

    int n = 2; // Number of states
    int m = 1; // Number of measurements

    for (int i = 0; i < ndim; ++i) {
        MatrixXd P(n, n);
        MatrixXd F(n, n);
        MatrixXd H(n, m);
        MatrixXd R(m, m);
        MatrixXd Q(m, m);

        P.setIdentity(m, m);
        F.setIdentity(m, m);
        H.setIdentity(m, n);
        R(0, 0) = 5;
        Q(0, 0) = 0.01;

        VectorXd x0v(2);
        x0v << x0[i], 0;
        filters[i].init(x0v, P, F, H, R, Q);
    }

    m_initialized = true;
}

std::vector<double> KalmanFilter1d::state() {
//    qDebug() << "KalmanFilter1d::state";

    std::vector<double> result;
    result.reserve(ndim);
    for (int i = 0; i < ndim; ++i) {
        result.emplace_back(filters[i].state()(0));
    }
    return result;
}

std::vector<double> KalmanFilter1d::step(std::vector<double> &x) {
//    qDebug() << "KalmanFilter1d::step";

    predict();
    update(x);
    return state();
}

void KalmanFilter1d::predict() {
//    qDebug() << "KalmanFilter1d::predict";

    for (int i = 0; i < ndim; ++i) {
        filters[i].predict();
    }
}

void KalmanFilter1d::update(std::vector<double> &x) {
//    qDebug() << "KalmanFilter1d::update";

    for (int i = 0; i < ndim; ++i) {
        VectorXd xv(2);
        xv << x[i], 0;
        filters[i].update(xv);
    }
}