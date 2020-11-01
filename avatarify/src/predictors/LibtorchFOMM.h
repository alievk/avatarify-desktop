#ifndef LIBTORCHFOMM_H
#define LIBTORCHFOMM_H

#include <QDir>
#include "LibtorchPredictor.h"

class LibtorchFOMM : public LibtorchPredictor {
public:
    LibtorchFOMM();

    void calibrate(QImage &drivingFrame);
    void requestCalibration();

private:
    void setSourceImageInternal(torch::Tensor &newSourceImage) override;

    torch::Tensor predictInternal(torch::Tensor &drivingImage) override;

    torch::Tensor FOMMEncoder(const torch::Tensor &image);

    std::pair<torch::Tensor, torch::Tensor> KPDetector(const torch::Tensor &image);

    torch::Tensor FOMMNoEncoderNoKPDetector(const torch::Tensor &kpDriving, const torch::Tensor &kpDrivingJacobian);

    void calibrate(torch::Tensor &kpDriving,
                   torch::Tensor &kpDrivingJacobian);

    const QString FOMMEncoderPath = QDir::homePath() + "/.avatarify/models/FOMMEncoder.pt";
    const QString KPDetectorPath = QDir::homePath() + "/.avatarify/models/KPDetector.pt";
    const QString FOMMNoEncoderNoKPDetectorPath = QDir::homePath() + "/.avatarify/models/FOMMNoEncoderNoKPDetector.pt";

    torch::jit::script::Module FOMMEncoderModule;
    torch::jit::script::Module KPDetectorModule;
    torch::jit::script::Module FOMMNoEncoderNoKPDetectorModule;

    torch::Tensor sourceImage;
    torch::Tensor sourceEncoded;
    torch::Tensor kpSource;
    torch::Tensor kpSourceJacobian;
    torch::Tensor kpInitial;
    torch::Tensor kpInitialJacobian;

    bool isCalibrated = false;
};


#endif //LIBTORCHFOMM_H
