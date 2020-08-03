#include "AkVCamBridge.h"

const std::wstring AkVCamBridge::description(L"Avatarify Camera");
const AkVCam::VideoFormat AkVCamBridge::format(AkVCam::PixelFormat::PixelFormatRGB24, 1280, 720);

AkVCamBridge::AkVCamBridge(): formats({format}) {
    allocateDevice();
}

void AkVCamBridge::allocateDevice() {
    m_ipcBridge.connectService(false);
    auto devices = m_ipcBridge.listDevices();
    std::cout << "List of devices:" << std::endl;
    for (const auto &d : devices) {
        std::cout << "\t" << d << std::endl;
    }
    if (!devices.empty()) {
        qDebug() << "There are " << devices.size() << " devices. Choose 0...";
        m_device = devices[0];
        m_ipcBridge.deviceEdit(m_device, L"Avatarify Camera", formats);
    } else {
        qDebug() << "No available devices. Need to create one...";
        m_device = m_ipcBridge.deviceCreate(description, formats);
    }
    bool isStarted = m_ipcBridge.deviceStart(m_device, format);
    if (isStarted) {
        std::cout << "Success initializing " << m_device << std::endl;
    } else {
        std::cout << "Error initializing " << m_device << std::endl;
    }
}

void AkVCamBridge::present(const QImage &generatedFrame) {
    if (m_device != "<EMPTY>") {
        AkVCam::VideoFrame videoFrame(format);
        videoFrame.data() = std::vector<uint8_t>(generatedFrame.constBits(), generatedFrame.constBits() + generatedFrame.sizeInBytes());
        m_ipcBridge.write(m_device, videoFrame);
    }
}