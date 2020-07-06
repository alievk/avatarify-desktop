#include <torch/script.h>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include "LibtorchPredictor.h"

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

    FOMMEncoderModule = torch::jit::load(FOMMEncoderPath.toStdString());
    KPDetectorModule = torch::jit::load(KPDetectorPath.toStdString());
    FOMMNoEncoderNoKPDetectorModule = torch::jit::load(FOMMNoEncoderNoKPDetectorPath.toStdString());
}

void LibtorchPredictor::setSourceImage(QString &avatarPath) {
    qDebug() << "LibtorchPredictor::setSourceImage";

    QImage avatar;
    avatar.load(avatarPath);
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
    }

    torch::Tensor generatedImage = FOMMNoEncoderNoKPDetector(kpDriving, kpDrivingJacobian);
    generatedImage = at::upsample_bilinear2d(generatedImage, {640, 480}, false);
    return tensorToQImage(generatedImage);
}

QImage LibtorchPredictor::tensorToQImage(const torch::Tensor &tensor) {
    QImage image;
    int dim = tensor.dim();
    if (dim != 4) {
        qFatal("dim must be 4.");
    }
    int width = tensor.size(3);
    int height = tensor.size(2);

    // fill QImage
    image = QImage(width, height, QImage::Format_ARGB32);
    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height; ++h) {
            int r = floor(tensor[0][0][h][w].item<float>() * 255.0);
            int g = floor(tensor[0][1][h][w].item<float>() * 255.0);
            int b = floor(tensor[0][2][h][w].item<float>() * 255.0);
            image.setPixel(w, h, qRgba(r, g, b, 0));
        }
    }
    return image;
}

torch::Tensor LibtorchPredictor::qimageToTensor(const QImage &image) {
    int width = image.width();
    int height = image.height();
    int channels = 3;
    qDebug() << width << " " << height << " " << channels;

    // create tensor
    torch::TensorOptions option(torch::kFloat32);
    torch::Tensor tensor = torch::zeros({1, channels, height, width}, option);//N C H W

    // fill tensor
    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height; ++h) {
            QRgb rgb = image.pixel(w, h);
            tensor[0][0][h][w] = qRed(rgb) / 255.0; //R
            tensor[0][1][h][w] = qGreen(rgb) / 255.0; //G
            tensor[0][2][h][w] = qBlue(rgb) / 255.0; //B
        }
    }

    return tensor;
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
    return std::pair<torch::Tensor, torch::Tensor>(outputs->elements()[0].toTensor(), outputs->elements()[1].toTensor());
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

