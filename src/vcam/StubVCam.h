#ifndef STUBVCAM_H
#define STUBVCAM_H


#include "AbstractVCam.h"

class StubVCam : public AbstractVCam {
public:
    void present(const QImage& generatedFrame) override;
};


#endif //STUBVCAM_H
