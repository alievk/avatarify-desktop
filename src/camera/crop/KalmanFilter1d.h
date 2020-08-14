//
// Created by vlivashkin on 14.08.2020.
//

#ifndef AVATARIFY_KALMANFILTER1D_H
#define AVATARIFY_KALMANFILTER1D_H


#include <vector>
#include "KalmanFilter.h"

class KalmanFilter1d {
public:
    KalmanFilter1d(int ndim, double r, double q);

    bool isInitialized() const;

    void setInitialized(bool initialized);

    void initFilters(std::vector<double> &x0);

    std::vector<double> state();

    std::vector<double> step(std::vector<double> &x);

private:
    int ndim = 1;
    double m_R = 5;
    double m_Q = 0.01;
    bool m_initialized = false;

    std::vector<KalmanFilter> filters;

    void predict();

    void update(std::vector<double> &x);
};


#endif //AVATARIFY_KALMANFILTER1D_H
