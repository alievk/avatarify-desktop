#include <QFileInfo>
#include <torch/script.h>
#include <QDebug>

#include "LibtorchFOMM.h"

LibtorchFOMM::LibtorchFOMM() {
    if (!QFileInfo::exists(FOMMEncoderPath)) {
        qWarning() << "FOMMEncoderPath doesn't exist";
    }
    if (!QFileInfo::exists(KPDetectorPath)) {
        qWarning() << "KPDetectorPath doesn't exist";
    }
    if (!QFileInfo::exists(FOMMNoEncoderNoKPDetectorPath)) {
        qWarning() << "FOMMNoEncoderNoKPDetectorPath doesn't exist";
    }

    torch::init_num_threads();
    FOMMEncoderModule = torch::jit::load(FOMMEncoderPath.toStdString());
    KPDetectorModule = torch::jit::load(KPDetectorPath.toStdString());
    FOMMNoEncoderNoKPDetectorModule = torch::jit::load(FOMMNoEncoderNoKPDetectorPath.toStdString());
}

void LibtorchFOMM::setSourceImageInternal(torch::Tensor &newSourceImage) {
    sourceImage = newSourceImage;

    sourceEncoded = FOMMEncoder(newSourceImage);
    auto kpAndJacobian = KPDetector(newSourceImage);
    kpSource = kpAndJacobian.first;
    kpSourceJacobian = kpAndJacobian.second;
}

torch::Tensor LibtorchFOMM::predictInternal(torch::Tensor &drivingImage) {
    auto kpAndJacobian = KPDetector(drivingImage);
    torch::Tensor kpDriving = kpAndJacobian.first;
    torch::Tensor kpDrivingJacobian = kpAndJacobian.second;

    if (!isCalibrated) {
        kpInitial = kpDriving;
        kpInitialJacobian = kpDrivingJacobian;
        isCalibrated = true;
    }

    torch::Tensor generatedImage = FOMMNoEncoderNoKPDetector(kpDriving, kpDrivingJacobian);
    return generatedImage;
}

torch::Tensor LibtorchFOMM::FOMMEncoder(const torch::Tensor &image) {
//    qDebug() << "LibtorchPredictor::FOMMEncoder";
    return FOMMEncoderModule.forward({image}).toTensor();
}

std::pair<torch::Tensor, torch::Tensor> LibtorchFOMM::KPDetector(const torch::Tensor &image) {
//    qDebug() << "LibtorchPredictor::KPDetector";
    auto outputs = KPDetectorModule.forward({image}).toTuple();
    return std::pair<torch::Tensor, torch::Tensor>(outputs->elements()[0].toTensor(),
                                                   outputs->elements()[1].toTensor());
}

torch::Tensor LibtorchFOMM::FOMMNoEncoderNoKPDetector(const torch::Tensor &kpDriving,
                                                      const torch::Tensor &kpDrivingJacobian) {
//    qDebug() << "LibtorchPredictor::FOMMNoEncoderNoKPDetector";
    std::vector<torch::jit::IValue> inputs = {
            sourceImage,
            sourceEncoded,
            kpDriving,
            kpDrivingJacobian,
            kpSource,
            kpSourceJacobian,
            kpInitial,
            kpInitialJacobian
    };
    return FOMMNoEncoderNoKPDetectorModule.forward(inputs).toTensor();
}


