#include <torch/script.h>
#include <QFileInfo>
#include <QDebug>
#include "LibtorchPredictor.h"

using namespace torch::indexing;

LibtorchPredictor::LibtorchPredictor() {
    if (not QFileInfo::exists(FOMMEncoderPath)) {
        qWarning() << "FOMMEncoderPath doesn't exist";
    }
    if (not QFileInfo::exists(KPDetectorPath)) {
        qWarning() << "KPDetectorPath doesn't exist";
    }
    if (not QFileInfo::exists(FOMMNoEncoderNoKPDetectorPath)) {
        qWarning() << "FOMMNoEncoderNoKPDetectorPath doesn't exist";
    }

    torch::init_num_threads();
    FOMMEncoderModule = torch::jit::load(FOMMEncoderPath.toStdString());
    KPDetectorModule = torch::jit::load(KPDetectorPath.toStdString());
    FOMMNoEncoderNoKPDetectorModule = torch::jit::load(FOMMNoEncoderNoKPDetectorPath.toStdString());
}

void LibtorchPredictor::setSourceImage(QString &avatarPath) {
    qDebug() << "LibtorchPredictor::setSourceImage";

    QImage avatar;
    avatar.load(avatarPath);
    avatar = avatar.convertToFormat(QImage::Format_RGB888);
    sourceImage = torch::upsample_bilinear2d(qimageToTensor(avatar), {256, 256}, false);
    sourceEncoded = FOMMEncoder(sourceImage);
    auto kpAndJacobian = KPDetector(sourceImage);
    kpSource = kpAndJacobian.first;
    kpSourceJacobian = kpAndJacobian.second;

    isSourceImageReady = true;
}

QImage LibtorchPredictor::predict(QImage &drivingFrame) {
    qDebug() << "LibtorchPredictor::predict";

    if (not isSourceImageReady) {
        qWarning() << "isSourceImageReady=false";
        return drivingFrame;
    }

    torch::Tensor drivingImage = qimageToTensor(drivingFrame);
    drivingImage = torch::upsample_bilinear2d(drivingImage, {256, 256}, false);

    auto kpAndJacobian = KPDetector(drivingImage);
    torch::Tensor kpDriving = kpAndJacobian.first;
    torch::Tensor kpDrivingJacobian = kpAndJacobian.second;

    if (not isCalibrated) {
        kpInitial = kpDriving;
        kpInitialJacobian = kpDrivingJacobian;
        isCalibrated = true;
        return drivingFrame;
    }

    torch::Tensor generatedImage = FOMMNoEncoderNoKPDetector(kpDriving, kpDrivingJacobian);
    generatedImage = at::upsample_bilinear2d(generatedImage, {640, 480}, false);
    return tensorToQImage(generatedImage);
}

QImage LibtorchPredictor::tensorToQImage(torch::Tensor &tensor) {
    int dim = tensor.dim();
    if (dim != 4) {
        qFatal("dim must be 4.");
    }
    int width = tensor.size(3);
    int height = tensor.size(2);

    tensor = (tensor * 255.0f).permute({0, 2, 3, 1}).to(torch::kUInt8).flatten();
    return QImage((uchar *)tensor.data_ptr(), width, height, QImage::Format_RGB888).copy();
}

torch::Tensor LibtorchPredictor::qimageToTensor(QImage &image) {
    int width = image.width();
    int height = image.height();
    int channels = 3;

    torch::TensorOptions options(torch::kUInt8);
    torch::Tensor tensor = torch::from_blob(image.bits(), {1, width, height, channels}, options);
    return (tensor / 255.0f).permute({0, 3, 1, 2}).to(torch::kFloat32);
}

torch::Tensor LibtorchPredictor::FOMMEncoder(const torch::Tensor &image) {
    qDebug() << "LibtorchPredictor::FOMMEncoder";

    std::vector<torch::jit::IValue> inputs;
    inputs.emplace_back(image);
    return FOMMEncoderModule.forward(inputs).toTensor();
}

std::pair<torch::Tensor, torch::Tensor> LibtorchPredictor::KPDetector(const torch::Tensor &image) {
    qDebug() << "LibtorchPredictor::KPDetector";

    std::vector<torch::jit::IValue> inputs;
    inputs.emplace_back(image);
    auto outputs = KPDetectorModule.forward(inputs).toTuple();
    return std::pair<torch::Tensor, torch::Tensor>(outputs->elements()[0].toTensor(),
                                                   outputs->elements()[1].toTensor());
}

torch::Tensor LibtorchPredictor::FOMMNoEncoderNoKPDetector(const torch::Tensor &kpDriving,
                                                           const torch::Tensor &kpDrivingJacobian) {
    qDebug() << "LibtorchPredictor::FOMMNoEncoderNoKPDetector";

    std::vector<torch::jit::IValue> inputs;
    inputs.emplace_back(sourceImage);
    inputs.emplace_back(sourceEncoded);
    inputs.emplace_back(kpDriving);
    inputs.emplace_back(kpDrivingJacobian);
    inputs.emplace_back(kpSource);
    inputs.emplace_back(kpSourceJacobian);
    inputs.emplace_back(kpInitial);
    inputs.emplace_back(kpInitialJacobian);
    return FOMMNoEncoderNoKPDetectorModule.forward(inputs).toTensor();
}

