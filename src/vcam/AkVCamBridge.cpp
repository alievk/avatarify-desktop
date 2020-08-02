#include "AkVCamBridge.h"

AkVCamBridge::AkVCamBridge() {
    m_ipcBridge.connectService(false);
    auto devices = m_ipcBridge.listDevices();

    std::cout << "List of devices:" << std::endl;
    for (const auto &d : devices) {
        std::cout << "\t" << d << std::endl;
    }
    m_device = devices[0];

    m_format = AkVCam::VideoFormat(AkVCam::PixelFormat::PixelFormatRGB24, 1280, 720);
    bool isStarted = m_ipcBridge.deviceStart(m_device, m_format);

    std::vector<AkVCam::VideoFormat> formats;
    formats.push_back(m_format);
    m_ipcBridge.deviceEdit(m_device, L"Avatarify Camera", formats);

    if (isStarted) {
        std::cout << "Success initializing " << devices[0];
    } else {
        std::cout << "Error initializing " << devices[0];
    }
}

AkVCamBridge::~AkVCamBridge() {

}

void AkVCamBridge::present(const QImage &generatedFrame) {
    AkVCam::VideoFrame videoFrame(m_format);
    videoFrame.data() = std::vector<uint8_t>(generatedFrame.constBits(), generatedFrame.constBits() + generatedFrame.sizeInBytes());
    m_ipcBridge.write(m_device, videoFrame);
}