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

    torch::NoGradGuard guard;

    torch::init_num_threads();
    FOMMEncoderModule = torch::jit::load(FOMMEncoderPath.toStdString(), device);
    KPDetectorModule = torch::jit::load(KPDetectorPath.toStdString(), device);
    FOMMNoEncoderNoKPDetectorModule = torch::jit::load(FOMMNoEncoderNoKPDetectorPath.toStdString(), device);
}

void LibtorchFOMM::setSourceImageInternal(torch::Tensor &newSourceImage) {
    sourceImage = newSourceImage;

    sourceEncoded = FOMMEncoder(newSourceImage);
    auto kpAndJacobian = KPDetector(newSourceImage);
    kpSource = kpAndJacobian.first;
    kpSourceJacobian = kpAndJacobian.second;

    // recalibrate. This is not mandatory there, but this allows user to recalibrate if they need (by toggling avatar)
    isCalibrated = false;
}

torch::Tensor LibtorchFOMM::predictInternal(torch::Tensor &drivingImage) {
    auto kpAndJacobian = KPDetector(drivingImage);
    torch::Tensor kpDriving = kpAndJacobian.first;
    torch::Tensor kpDrivingJacobian = kpAndJacobian.second;

    if (!isCalibrated) {
        kpInitial = kpDriving.clone();
        kpInitialJacobian = kpDrivingJacobian.clone();
        isCalibrated = true;
    }

    torch::Tensor generatedImage = FOMMNoEncoderNoKPDetector(kpDriving, kpDrivingJacobian);
    return generatedImage;
}

torch::Tensor LibtorchFOMM::FOMMEncoder(const torch::Tensor &image) {
    //    qDebug() << "LibtorchPredictor::FOMMEncoder";
    torch::NoGradGuard guard;
    return FOMMEncoderModule.forward({image}).toTensor();
}

std::pair<torch::Tensor, torch::Tensor> LibtorchFOMM::KPDetector(const torch::Tensor &image) {
//    qDebug() << "LibtorchPredictor::KPDetector";
    torch::NoGradGuard guard;
    auto outputs = KPDetectorModule.forward({image}).toTuple();
    return std::pair<torch::Tensor, torch::Tensor>(outputs->elements()[0].toTensor(),
                                                   outputs->elements()[1].toTensor());
}

torch::Tensor LibtorchFOMM::FOMMNoEncoderNoKPDetector(const torch::Tensor &kpDriving,
                                                      const torch::Tensor &kpDrivingJacobian) {
    //    qDebug() << "LibtorchPredictor::FOMMNoEncoderNoKPDetector";
    torch::NoGradGuard guard;
    return FOMMNoEncoderNoKPDetectorModule.forward({sourceImage, sourceEncoded,
                                                    kpDriving, kpDrivingJacobian,
                                                    kpSource, kpSourceJacobian,
                                                    kpInitial, kpInitialJacobian}).toTensor();
}


