#ifndef LIBTORCHIDENTITYPREDICTOR_H
#define LIBTORCHIDENTITYPREDICTOR_H


#include "LibtorchPredictor.h"

class LibtorchIdentityPredictor : public LibtorchPredictor {
public:
    LibtorchIdentityPredictor();
private:
    void setSourceImageInternal(torch::Tensor &newSourceImage) override;

    torch::Tensor predictInternal(torch::Tensor &drivingImage) override;
};


#endif //LIBTORCHIDENTITYPREDICTOR_H
