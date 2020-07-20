#ifndef DIRECTSHOWVCAM_H
#define DIRECTSHOWVCAM_H

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "AbstractVCam.h"

using namespace boost::interprocess;

class DirectShowVCam : public AbstractVCam {
public:
    DirectShowVCam();

    void present(const QImage &frame) override;

private:
    shared_memory_object shm;
};


#endif //DIRECTSHOWVCAM_H
