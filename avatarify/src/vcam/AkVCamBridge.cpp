#include "AkVCamBridge.h"

#include <QDir>
#include <QThread>

const std::wstring AkVCamBridge::description(L"Avatarify Camera");
const std::vector<AkVCam::PixelFormat> AkVCamBridge::pixelFormats({AkVCam::PixelFormat::PixelFormatRGB32,
                                                                   AkVCam::PixelFormat::PixelFormatRGB24,
                                                                   AkVCam::PixelFormat::PixelFormatUYVY,
                                                                   AkVCam::PixelFormat::PixelFormatYUY2});

AkVCamBridge::AkVCamBridge() : format(AkVCam::PixelFormat::PixelFormatRGB24, width, height, {{uint32_t(fps), uint32_t(1)}}) {
    for (auto &pixelFormat : pixelFormats) {
        supportedFormats.emplace_back(AkVCam::VideoFormat(pixelFormat, width, height, {{uint32_t(fps), uint32_t(1)}}));
    };
    m_available = allocateDevice();
}

bool AkVCamBridge::allocateDevice() {
    m_ipcBridge.connectService(false);
    m_ipcBridge.setDriverPaths({QDir::currentPath().toStdWString()});
    auto devices = m_ipcBridge.listDevices();
    std::cout << "List of devices:" << std::endl;
    for (const auto &d : devices) {
        std::cout << "\t" << d << std::endl;
    }
    
    bool isStarted{false};
    if (devices.empty()) {
        qDebug() << "No available devices. Need to create one..." << supportedFormats.size();
        m_ipcBridge.deviceCreate(description, supportedFormats);
        devices = m_ipcBridge.listDevices();
        qDebug() << devices.size();
        if (!devices.empty()) {
            m_device = devices[0];
            qDebug() << m_device.c_str();
            for (int i = 1; i < 4; ++i) {
                QThread::sleep(4 * i);
                if ((isStarted = m_ipcBridge.deviceStart(m_device, format))) {
                    break;
                }
            }
            if (!isStarted) {
                return false;
            }
        }
        //m_device = m_ipcBridge.deviceCreate(description, supportedFormats);
        //if (m_device.empty()) { // error during creating the device
        //    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        //    std::cout << "ipcBridge: " << converter.to_bytes(m_ipcBridge.errorMessage()) << std::endl;
        //    return false;
        //}
    } else {
        qDebug() << "There are " << devices.size() << " devices. Choose 0...";
        m_device = devices[0];
        //m_ipcBridge.deviceEdit(m_device, L"Avatarify Camera", supportedFormats);
    }
    if (!isStarted) {
        isStarted = m_ipcBridge.deviceStart(m_device, format);
    }
    if (isStarted) {
        std::cout << "Success initializing " << m_device << std::endl;
        return true;
    } else {
        std::cout << "Error initializing " << m_device << std::endl;
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        std::cout << "ipcBridge: " << converter.to_bytes(m_ipcBridge.errorMessage()) << std::endl;
        return false;
    }
}

void AkVCamBridge::present(const QImage &generatedFrame) {
    if (m_available && m_stream) {
        AkVCam::VideoFrame videoFrame(format);
        videoFrame.data() = std::vector<uint8_t>(generatedFrame.constBits(),
                                                 generatedFrame.constBits() + generatedFrame.sizeInBytes());
        m_ipcBridge.write(m_device, videoFrame);
    }
}
