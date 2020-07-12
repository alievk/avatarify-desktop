
#include <QDebug>
#include "LibtorchPredictor.h"


void LibtorchPredictor::setSourceImage(QString &avatarPath) {
    qDebug() << "LibtorchPredictor::setSourceImageInternal";

    QImage avatar;
    avatar.load(avatarPath);
    avatar = avatar.convertToFormat(QImage::Format_RGB888);

    isSourceImageReady = false;

    torch::Tensor newSourceImage = qimageToTensor(avatar);
    newSourceImage = torch::upsample_bilinear2d(newSourceImage, {480, 480}, false);
    newSourceImage = torch::nn::functional::pad(newSourceImage, torch::nn::functional::PadFuncOptions({80, 80}));
    newSourceImage = torch::upsample_bilinear2d(newSourceImage, {256, 256}, false);

    setSourceImageInternal(newSourceImage);

    isSourceImageReady = true;
}

QImage LibtorchPredictor::predict(QImage &drivingFrame) {
//    qDebug() << "LibtorchPredictor::predictInternal";

    if (not isSourceImageReady) {
        qWarning() << "isSourceImageReady=false";
        return drivingFrame;
    }

    torch::Tensor drivingImage = qimageToTensor(drivingFrame);
    drivingImage = torch::upsample_bilinear2d(drivingImage, {256, 256}, false);
    torch::Tensor generatedImage = predictInternal(drivingImage);
    generatedImage = at::upsample_bilinear2d(generatedImage, {640, 480}, false);
    return tensorToQImage(generatedImage);
}

QImage LibtorchPredictor::tensorToQImage(torch::Tensor &tensor) {
    int width = tensor.size(3);
    int height = tensor.size(2);

    tensor = (tensor * 255.0f).permute({0, 2, 3, 1}).to(torch::kUInt8).flatten();
    return QImage((uchar *) tensor.data_ptr(), width, height, QImage::Format_RGB888).copy();
}

torch::Tensor LibtorchPredictor::qimageToTensor(QImage &image) {
    int width = image.width();
    int height = image.height();

    torch::TensorOptions options(torch::kUInt8);
    torch::Tensor tensor = torch::from_blob(image.bits(), {1, width, height, 3}, options);
    return (tensor / 255.0f).permute({0, 3, 1, 2}).to(torch::kFloat32);
}


