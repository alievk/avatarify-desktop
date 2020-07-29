#include "DriverVCamBridge.h"

#define IOCTL_IMAGE    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x4000, METHOD_BUFFERED, FILE_ANY_ACCESS)

DriverVCamBridge::DriverVCamBridge() {
    WCHAR DeviceLink[] = L"\\\\.\\cloudphone";
    hdevice = CreateFileW(
            DeviceLink,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_OFFLINE,
            nullptr
    );
    if (hdevice == INVALID_HANDLE_VALUE) {
        printf("Unable to open UsbcameraFilter device - error %lu\n", GetLastError());
        return;
    }
}

DriverVCamBridge::~DriverVCamBridge() {
    CloseHandle(hdevice);
}

void DriverVCamBridge::present(const QImage &frame) {
//    qDebug() << "DirectShowVCam::present" << frame.width() << "x" << frame.height() << "@" << frame.depth();
//    qDebug() << frame.format();

    QImage mirorredFrame = frame.mirrored();
    const uchar *buf = mirorredFrame.constBits();

    DWORD dwRet;
    if (!DeviceIoControl(hdevice,
                         IOCTL_IMAGE,
                         (LPVOID) buf,
                         sizeof(buf),
                         nullptr,
                         0,
                         &dwRet,
                         nullptr)) {
        printf("DeviceIOControl Fail!! \n");
        CloseHandle(hdevice);
        return;
    }
}