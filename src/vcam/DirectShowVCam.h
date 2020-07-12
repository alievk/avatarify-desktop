#ifndef DIRECTSHOWVCAM_H
#define DIRECTSHOWVCAM_H


#include "AbstractVCam.h"

class DirectShowVCam : public AbstractVCam {
public:
    void present(const QImage &generatedFrame) override;
};


#endif //DIRECTSHOWVCAM_H
