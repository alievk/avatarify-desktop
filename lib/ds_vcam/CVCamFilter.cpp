#include "CVCamFilter.h"
#include "CVCamOutStream.h"
#include "utils/Log.h"


//////////////////////////////////////////////////////////////////////////
//  CSimpleVirtualCamFilter is the source filter which masquerades as a capture device
//////////////////////////////////////////////////////////////////////////
CUnknown *WINAPI CVCamFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr) {
    L_(linfo) << "CVCamFilter::CreateInstance";

    ASSERT(phr);
    CUnknown * punk = new CVCamFilter(lpunk, phr);
    return punk;
}

CVCamFilter::CVCamFilter(LPUNKNOWN lpunk, HRESULT *phr) : CSource(NAME("Avatarify Camera v0.1.2"), lpunk, CLSID_VCamFilter) {
    L_(linfo) << "CVCamFilter::CVCamFilter";

    ASSERT(phr);
    CAutoLock cAutoLock(&m_cStateLock);

    // Create the one and only output pin
    m_paStreams = (CSourceStream **) new CVCamOutStream *[1];
    m_paStreams[0] = new CVCamOutStream(phr, this, L"Capture");
}

HRESULT CVCamFilter::QueryInterface(REFIID riid, void **ppv) {
    L_(linfo) << "CVCamFilter::QueryInterface";

    //Forward request for IAMStreamConfig & IKsPropertySet to the pin
    if (riid == _uuidof(IAMStreamConfig) || riid == _uuidof(IKsPropertySet))
        return m_paStreams[0]->QueryInterface(riid, ppv);
    else
        return CSource::QueryInterface(riid, ppv);
}
