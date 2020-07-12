#ifndef LIBTORCHPREDICTOR_H
#define LIBTORCHPREDICTOR_H

#include <torch/torch.h>
#include "Predictor.h"

class LibtorchPredictor : Predictor {
public:
    void setSourceImage(QString &avatarPath) override;

    QImage predict(QImage &drivingFrame) override;

private:
    static QImage tensorToQImage(torch::Tensor &tensor);

    static torch::Tensor qimageToTensor(QImage &image);

    virtual void setSourceImageInternal(torch::Tensor &avatar) = 0;

    virtual torch::Tensor predictInternal(torch::Tensor &drivingImage) = 0;

    bool isSourceImageReady = false;
};


#endif //LIBTORCHPREDICTOR_H
