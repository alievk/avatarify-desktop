#ifndef AKVCAM_H
#define AKVCAM_H

#include <iostream>

#include "lib/akvirtualcamera/VCamUtils/src/ipcbridge.h"
#include "lib/akvirtualcamera/VCamUtils/src/image/videoformat.h"
#include "lib/akvirtualcamera/VCamUtils/src/image/videoframe.h"

#include "AbstractVCamInterface.h"
#include "QDebug"

class AkVCamBridge : public AbstractVCamInterface {
public:
    AkVCamBridge();

    ~AkVCamBridge();

    void present(const QImage &generatedFrame) override;

private:
    AkVCam::IpcBridge m_ipcBridge;
    AkVCam::VideoFormat m_format;
    std::string m_device;
};


#endif //AKVCAM_H
