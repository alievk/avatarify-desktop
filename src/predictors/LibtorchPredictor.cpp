
#include <QDebug>
#include "LibtorchPredictor.h"

const torch::Device LibtorchPredictor::device((torch::cuda::is_available()) ? torch::kCUDA : torch::kCPU);

void LibtorchPredictor::setSourceImage(QString &avatarPath) {
    qDebug() << "LibtorchPredictor::setSourceImageInternal";

    QImage avatar;
    avatar.load(avatarPath);
    avatar = avatar.convertToFormat(QImage::Format_RGB888);

    isSourceImageReady = false;

    torch::Tensor newSourceImage = qimageToTensor(avatar);
    newSourceImage = torch::upsample_bilinear2d(newSourceImage, {480, 480}, true);
    newSourceImage = torch::nn::functional::pad(newSourceImage, torch::nn::functional::PadFuncOptions({80, 80}));
    newSourceImage = torch::upsample_bilinear2d(newSourceImage, {256, 256}, true);

    setSourceImageInternal(newSourceImage);

    isSourceImageReady = true;
}

QImage LibtorchPredictor::predict(QImage &drivingFrame) {
//    qDebug() << "LibtorchPredictor::predictInternal";

    if (!isSourceImageReady) {
        qWarning() << "isSourceImageReady=false";
        return QImage();
    }

//    if (drivingFrame.width() != 1280 || drivingFrame.height() != 720) {
//        qWarning() << "wrong input size: " << drivingFrame.width() << "x" << drivingFrame.height();
//        return QImage(QSize(1280, 720), QImage::Format_RGB888);
//    }

    torch::Tensor drivingImage = qimageToTensor(drivingFrame);
    drivingImage = drivingImage.slice(3, 160, -160);
    drivingImage = torch::upsample_bilinear2d(drivingImage, {256, 256}, false);
    torch::Tensor generatedImage = predictInternal(drivingImage);
    generatedImage = torch::upsample_bilinear2d(generatedImage, {720, 960}, false);
    generatedImage = torch::nn::functional::pad(generatedImage, torch::nn::functional::PadFuncOptions({160, 160}));
    return tensorToQImage(generatedImage);
}

torch::Tensor LibtorchPredictor::qimageToTensor(QImage &image) {
    int height = image.height();
    int width = image.width();

    torch::TensorOptions options(torch::kUInt8);
    torch::Tensor tensor = torch::from_blob(image.bits(), {1, height, width, 3}, options);
    return (tensor / 255.0f).permute({0, 3, 1, 2}).to(torch::kFloat32).to(device);  // N C H W
}

QImage LibtorchPredictor::tensorToQImage(torch::Tensor &tensor) {
    int height = tensor.size(2);
    int width = tensor.size(3);

    tensor = (tensor * 255.0f).permute({0, 2, 3, 1}).to(torch::kUInt8).flatten().cpu();
    return QImage((uchar *) tensor.data_ptr(), width, height, QImage::Format_RGB888).copy();
}
