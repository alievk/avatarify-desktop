#include <QDebug>
#include "AbstractVCamInterface.h"


bool AbstractVCamInterface::enabled() const {
    return m_enabled;
}

void AbstractVCamInterface::setEnabled(bool enabled) {
    qDebug() << "AbstractVCam::setEnabled " << enabled;
    m_enabled = enabled;
}
