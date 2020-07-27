#include "avshws.h"

/**************************************************************************
    LOCKED CODE
**************************************************************************/

#ifdef ALLOC_PRAGMA
#pragma code_seg()
#endif // ALLOC_PRAGMA


void CImageSynthesizer::UpdateFrame() const {
//    if (m_Width == 640 && m_Height == 480 && m_bytesPerPixel == 3) {
//        try {
//            mapped_region region(m_shm, read_only);
//            if (region.get_size() == 640 * 480 * 3) {
//                auto *mem = static_cast<unsigned char *>(region.get_address());
//                std::copy(mem, mem + min(640 * 480 * 3, region.get_size()), m_Buffer);
//                return;
//            }
//        } catch (std::exception &e) {
//            // logging... but how?
//        }
//    }

    // fallback
    for (int i = 0; i < m_Width * m_Height * m_bytesPerPixel; ++i)
        m_Buffer[i] = rand();
}
