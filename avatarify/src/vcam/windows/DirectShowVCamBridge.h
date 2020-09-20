#ifndef DIRECTSHOWVCAM_H
#define DIRECTSHOWVCAM_H

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <src/vcam/AbstractVCamInterface.h>

using namespace boost::interprocess;

class DirectShowVCamBridge : public AbstractVCamInterface {
public:
    DirectShowVCamBridge();

    void present(const QImage &frame) override;

private:
    shared_memory_object shm;
};


#endif //DIRECTSHOWVCAM_H