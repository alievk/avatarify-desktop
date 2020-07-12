#ifndef LIBTORCHFOMM_H
#define LIBTORCHFOMM_H


#include "LibtorchPredictor.h"

class LibtorchFOMM : public LibtorchPredictor {
public:
    LibtorchFOMM();

private:
    void setSourceImageInternal(torch::Tensor &newSourceImage) override;

    torch::Tensor predictInternal(torch::Tensor &drivingImage) override;

    torch::Tensor FOMMEncoder(const torch::Tensor &image);

    std::pair<torch::Tensor, torch::Tensor> KPDetector(const torch::Tensor &image);

    torch::Tensor FOMMNoEncoderNoKPDetector(const torch::Tensor &kpDriving, const torch::Tensor &kpDrivingJacobian);

    QString FOMMEncoderPath = "/Users/vlivashkin/.avatarify/models/FOMMEncoder.pt";
    QString KPDetectorPath = "/Users/vlivashkin/.avatarify/models/KPDetector.pt";
    QString FOMMNoEncoderNoKPDetectorPath = "/Users/vlivashkin/.avatarify/models/FOMMNoEncoderNoKPDetector.pt";

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
