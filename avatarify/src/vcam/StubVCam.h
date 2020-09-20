#ifndef STUBVCAM_H
#define STUBVCAM_H


#include "AbstractVCamInterface.h"

class StubVCam : public AbstractVCamInterface {
public:
    void present(const QImage& generatedFrame) override;
};


#endif //STUBVCAM_H
