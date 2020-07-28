#include "avshws.h"

/**************************************************************************
    LOCKED CODE
**************************************************************************/

#ifdef ALLOC_PRAGMA
#pragma code_seg()
#endif // ALLOC_PRAGMA


void CImageSynthesizer::UpdateFrame() const {
    if (m_Width == 640 && m_Height == 480 && m_bytesPerPixel == 3) {
        for (long i = 0; i < long(m_Width) * long(m_Height) * long(m_bytesPerPixel); ++i) {
            m_outBuffer[i] = m_inBuffer[i];
        }
        return;
    }

    // fallback
    for (long i = 0; i < long(m_Width) * long(m_Height) * long(m_bytesPerPixel); ++i) {
        if (i % 3 != 0) {
            m_outBuffer[i] = m_inBuffer[i];
        } else {
            m_outBuffer[i] = (UCHAR) rand();
        }
    }
}