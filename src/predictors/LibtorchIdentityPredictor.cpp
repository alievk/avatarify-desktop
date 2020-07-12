#include "LibtorchIdentityPredictor.h"

void LibtorchIdentityPredictor::setSourceImageInternal(torch::Tensor &newSourceImage) {

}

torch::Tensor LibtorchIdentityPredictor::predictInternal(torch::Tensor &drivingImage) {
    return drivingImage;
}
