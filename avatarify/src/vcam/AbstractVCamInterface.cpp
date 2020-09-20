#include <QDebug>
#include "AbstractVCamInterface.h"

bool AbstractVCamInterface::available() const {
    return m_available;
}

bool AbstractVCamInterface::stream() const {
    return m_stream;
}

void AbstractVCamInterface::setStream(bool stream) {
    qDebug() << "AbstractVCam::setStream " << stream;
    m_stream = stream;
}

