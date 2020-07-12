#include <QDebug>
#include "AbstractVCam.h"


bool AbstractVCam::enabled() const {
    return m_enabled;
}

void AbstractVCam::setEnabled(bool enabled) {
    qDebug() << "AbstractVCam::setEnabled " << enabled;
    m_enabled = enabled;
}
