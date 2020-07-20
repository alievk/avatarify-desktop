#ifndef SVCAM_VCAMFILTER_H
#define SVCAM_VCAMFILTER_H


#include "stdafx.h"

EXTERN_C const GUID CLSID_VCamFilter;


//------------------------------------------------------------------------------
// Class CSimpleVirtualCamFilter
//
// This is the main class for the virtual cam filter. It inherits from
// CSource, the DirectShow base class for source filters.
//------------------------------------------------------------------------------
class CVCamFilter : public CSource {
public:

    // The only allowed way to create Bouncing balls!
    static CUnknown *WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

    STDMETHODIMP QueryInterface(REFIID riid, void **ppv) override;

    IFilterGraph *GetGraph() { return m_pGraph; }

private:

    // It is only allowed to to create these objects with CreateInstance
    CVCamFilter(LPUNKNOWN lpunk, HRESULT *phr);
};

#endif // SVCAM_VCAMFILTER_H