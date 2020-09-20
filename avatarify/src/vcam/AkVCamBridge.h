#ifndef AKVCAM_H
#define AKVCAM_H

#include <iostream>
#include <locale>
#include <codecvt>

#include "ipcbridge.h"
#include "image/videoformat.h"
#include "image/videoframe.h"

#include "AbstractVCamInterface.h"
#include "QDebug"

class AkVCamBridge : public AbstractVCamInterface {
public:
    AkVCamBridge();

    void present(const QImage &generatedFrame) override;

private:
    static const std::wstring description;
    static const std::vector<AkVCam::PixelFormat> pixelFormats;
    static const int width = 1280;
    static const int height = 720;
    static const int fps = 30;
    std::vector<AkVCam::VideoFormat> supportedFormats;
    AkVCam::VideoFormat format;

    bool allocateDevice();

    AkVCam::IpcBridge m_ipcBridge;
    std::string m_device;
};


#endif //AKVCAM_H
