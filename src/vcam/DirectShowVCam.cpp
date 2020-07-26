#include "DirectShowVCam.h"
#include <QDebug>

DirectShowVCam::DirectShowVCam() {
    shm = shared_memory_object(open_or_create, "afy_shm", read_write);
    shm.truncate(640 * 480 * 3);
}

void DirectShowVCam::present(const QImage &frame) {
//    qDebug() << "DirectShowVCam::present" << frame.width() << "x" << frame.height() << "@" << frame.depth();
//    qDebug() << frame.format();

    QImage mirorredFrame = frame.mirrored();
    const uchar *bits = mirorredFrame.constBits();

    mapped_region region(shm, read_write);
    auto *mem = static_cast<unsigned char *>(region.get_address());
    std::copy(bits, bits + frame.sizeInBytes(), mem);
}