#include "DirectShowVCam.h"
#include <QDebug>

#define IOCTL_IMAGE    CTL_CODE(FILE_DEVICE_UNKNOWN,0x4000,METHOD_BUFFERED,FILE_ANY_ACCESS)

DirectShowVCam::DirectShowVCam() {

}

DirectShowVCam::~DirectShowVCam() {

}

void DirectShowVCam::present(const QImage &frame) {
//    qDebug() << "DirectShowVCam::present" << frame.width() << "x" << frame.height() << "@" << frame.depth();
//    qDebug() << frame.format();

    QImage mirorredFrame = frame.mirrored();
    const uchar *buf = mirorredFrame.constBits();

    WCHAR DeviceLink[] = L"\\\\.\\cloudphone";
    HANDLE hdevice = CreateFileW(
            DeviceLink,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_OFFLINE,
            nullptr
    );
    if (hdevice == INVALID_HANDLE_VALUE) {
        printf("Unable to open UsbcameraFilter device - error %d\n", GetLastError());
        return;
    }

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

    CloseHandle(hdevice);

    return;
}