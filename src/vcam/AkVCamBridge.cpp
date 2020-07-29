#include "AkVCamBridge.h"

AkVCamBridge::AkVCamBridge() {
    ipcb.connectService(false);
    devices = ipcb.listDevices();

    std::cout << "List of devices:" << std::endl;
    for (const auto &d : devices) {
        std::cout << "\t" << d << std::endl;
    }

    if (!ipcb.deviceStart(devices[0], AkVCam::VideoFormat(AkVCam::PixelFormat::PixelFormatRGB24, 640, 480))) {
        std::cout << "Error initializing " << devices[0];
    }
}

AkVCamBridge::~AkVCamBridge() {

}

void AkVCamBridge::present(const QImage &generatedFrame) {
    AkVCam::VideoFrame videoFrame;
    videoFrame.data() = redFrame();
    ipcb.write(devices[0], videoFrame);
}

AkVCam::VideoData AkVCamBridge::redFrame() {
    static AkVCam::VideoData frame;
    if (frame.empty()) {
        frame.reserve(640 * 480 * 3);
        for (int i = 0; i < 640; ++i)
            for (int j = 0; j < 480; ++j) {
                frame.push_back(0xFF);
                frame.push_back(0x00);
                frame.push_back(0x00);
            }
    }
    return frame;
}