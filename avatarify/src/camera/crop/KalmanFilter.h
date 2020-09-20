#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <vector>
#include <Eigen/Dense>
#include <QDebug>


using Eigen::MatrixXd;
using Eigen::VectorXd;

class KalmanFilter {
public:
    KalmanFilter();

    virtual ~KalmanFilter();

    bool isInitialized() const;

    void setInitialized(bool initialized);

    void init(VectorXd &x0, MatrixXd &P, MatrixXd &F, MatrixXd &H, MatrixXd &R, MatrixXd &Q);

    VectorXd &state();

    void predict();

    void update(const VectorXd &z);

    VectorXd &step(const VectorXd &z);

private:
    bool m_initialized = false;

    VectorXd m_x;  // state vector
    MatrixXd m_P;  // state covariance matrix
    MatrixXd m_F;  // state transition matrix
    MatrixXd m_H;  // measurement matrix
    MatrixXd m_R;  // measurement covariance matrix
    MatrixXd m_Q;  // process covariance matrix

};


#endif //KALMANFILTER_H
