#ifndef DRIVERVCAM_H
#define DRIVERVCAM_H

#include <src/vcam/AbstractVCamInterface.h>
#include <windows.h>

class DriverVCamBridge : public AbstractVCamInterface {
public:
    DriverVCamBridge();

    ~DriverVCamBridge() override;

    void present(const QImage &frame) override;

private:
    HANDLE hdevice;
};

#endif //DRIVERVCAM_H
