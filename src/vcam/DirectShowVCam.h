#ifndef DIRECTSHOWVCAM_H
#define DIRECTSHOWVCAM_H

#define HELLO_DRV_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

#include <cstdio>
#include <windows.h>
#include "AbstractVCam.h"

const char kDevName[] = R"(\\.\HelloDev)";

class DirectShowVCam : public AbstractVCam {
public:
    DirectShowVCam();

    ~DirectShowVCam() override;

    void present(const QImage &frame) override;

private:
    HANDLE dev;
};


#endif //DIRECTSHOWVCAM_H
