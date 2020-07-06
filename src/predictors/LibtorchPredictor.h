#ifndef LIBTORCHPREDICTOR_H
#define LIBTORCHPREDICTOR_H


#include "Predictor.h"

class LibtorchPredictor : Predictor {
public:
    LibtorchPredictor();

    void setSourceImage(QString &avatarPath) override;

    QImage predict(QImage &frame) override;

private:
    static QImage tensorToQImage(const torch::Tensor &tensor);

    static torch::Tensor qimageToTensor(const QImage &image);

    torch::jit::script::Module FOMMEncoder;
    torch::jit::script::Module KPDetector;
    torch::jit::script::Module FOMMNoEncoder;
    torch::Tensor encodedAvatar;
};


#endif //LIBTORCHPREDICTOR_H
