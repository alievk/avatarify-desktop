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

    void present(const QImage &generatedFrame) override;

private:
    static const std::wstring description;
    static const AkVCam::VideoFormat format;

    const std::vector<AkVCam::VideoFormat> formats;

    void allocateDevice();

    AkVCam::IpcBridge m_ipcBridge;
    std::string m_device = "<EMPTY>";
};


#endif //AKVCAM_H
