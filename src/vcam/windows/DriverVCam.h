#ifndef DRIVERVCAM_H
#define DRIVERVCAM_H

#include <src/vcam/AbstractVCam.h>
#include <windows.h>

class DriverVCam : public AbstractVCam {
public:
    DriverVCam();

    ~DriverVCam() override;

    void present(const QImage &frame) override;

private:
    HANDLE hdevice;
};

#endif //DRIVERVCAM_H
