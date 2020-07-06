#include <torch/torch.h>
#include <torch/script.h>
#include "LibtorchPredictor.h"

LibtorchPredictor::LibtorchPredictor() {
    FOMMEncoder = torch::jit::load("/home/wxy/QtWork/QtTorch/model.pt");
    KPDetector = torch::jit::load("/home/wxy/QtWork/QtTorch/model.pt");
    FOMMNoEncoder = torch::jit::load("/home/wxy/QtWork/QtTorch/model.pt");
}

void LibtorchPredictor::setSourceImage(QString &avatarPath) {
    QImage avatar;
    avatar.load(avatarPath);
    torch::Tensor avatarTensor = qimageToTensor(avatar);
    avatarTensor = at::upsample_bilinear2d(avatarTensor, {256, 256}, false);

    std::vector<torch::jit::IValue> inputs;
    inputs.emplace_back(avatarTensor);
    encodedAvatar = FOMMEncoder.forward(inputs).toTensor();
}

QImage LibtorchPredictor::predict(QImage &frame) {
    torch::Tensor frameTensor = qimageToTensor(frame);
    frameTensor = at::upsample_bilinear2d(frameTensor, {256, 256}, false);

    std::vector<torch::jit::IValue> inputs;
    inputs.emplace_back(frameTensor);
    torch::Tensor result = KPDetector.forward(inputs).toTensor();
    result = at::upsample_bilinear2d(frameTensor, {640, 480}, false);
    return tensorToQImage(result);
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
    image = QImage(width, height, QImage::Format_RGB888);
    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height; ++h) {
            int r = tensor[0][0][h][w].item<float>() * 255.0;
            int g = tensor[0][1][h][w].item<float>() * 255.0;
            int b = tensor[0][2][h][w].item<float>() * 255.0;
            QRgb rgb = qRgb(r, g, b);
            image.setPixel(w, h, rgb);
        }
    }

    return QImage();
}

torch::Tensor LibtorchPredictor::qimageToTensor(const QImage &image) {
    int width = image.width();
    int height = image.height();
    int depth = image.depth();
    int channels = depth / 8;

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

