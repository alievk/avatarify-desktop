#include <QDebug>
#include "LibtorchIdentityPredictor.h"

LibtorchIdentityPredictor::LibtorchIdentityPredictor() { // : LibtorchPredictor(torch::Device(torch::kCPU))
    isSourceImageReady = true;
}

void LibtorchIdentityPredictor::setSourceImageInternal(torch::Tensor &newSourceImage) {
    qDebug() << "LibtorchIdentityPredictor::setSourceImageInternal";
}

torch::Tensor LibtorchIdentityPredictor::predictInternal(torch::Tensor &drivingImage) {
    return drivingImage;
}
