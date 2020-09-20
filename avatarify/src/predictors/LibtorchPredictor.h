#ifndef LIBTORCHPREDICTOR_H
#define LIBTORCHPREDICTOR_H

#include <torch/torch.h>
#include "Predictor.h"

class LibtorchPredictor : public Predictor {
public:
    void setSourceImage(QString &avatarPath) override;

    QImage predict(QImage &drivingFrame) override;

protected:
    static const torch::Device device;
    bool isSourceImageReady = false;

private:
    static torch::Tensor qimageToTensor(QImage &image);

    static QImage tensorToQImage(torch::Tensor &tensor);

    virtual void setSourceImageInternal(torch::Tensor &avatar) = 0;

    virtual torch::Tensor predictInternal(torch::Tensor &drivingImage) = 0;
};


#endif //LIBTORCHPREDICTOR_H
