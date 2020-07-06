#ifndef LIBTORCHPREDICTOR_H
#define LIBTORCHPREDICTOR_H

#include <torch/torch.h>
#include "Predictor.h"

class LibtorchPredictor : Predictor {
public:
    LibtorchPredictor();

    void setSourceImage(QString &avatarPath) override;

    QImage predict(QImage &drivingFrame) override;

private:
    static QImage tensorToQImage(const torch::Tensor &tensor);

    static torch::Tensor qimageToTensor(const QImage &image);

    torch::Tensor FOMMEncoder(const torch::Tensor &sourceImage);

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

    bool isSourceImageReady = false;
    bool isCalibrated = false;
};


#endif //LIBTORCHPREDICTOR_H
