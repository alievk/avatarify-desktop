#ifndef AKVCAM_H
#define AKVCAM_H

#include <iostream>

#include <lib/akvirtualcamera/VCamUtils/src/ipcbridge.h>
#include <lib/akvirtualcamera/VCamUtils/src/image/videoformat.h>
#include <lib/akvirtualcamera/VCamUtils/src/image/videoframe.h>

#include "AbstractVCamInterface.h"

class AkVCamBridge : public AbstractVCamInterface {
public:
    AkVCamBridge();

    ~AkVCamBridge();

    void present(const QImage &generatedFrame) override;

private:
    static AkVCam::VideoData redFrame();

    AkVCam::IpcBridge ipcb;
    std::vector<std::string> devices;
};


#endif //AKVCAM_H
