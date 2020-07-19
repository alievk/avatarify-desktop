#include "CVCamFilter.h"
#include "CVCamOutStream.h"


//////////////////////////////////////////////////////////////////////////
//  CSimpleVirtualCamFilter is the source filter which masquerades as a capture device
//////////////////////////////////////////////////////////////////////////
CUnknown *WINAPI CVCamFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr) {
    ASSERT(phr);
    CUnknown *punk = new CVCamFilter(lpunk, phr);
    return punk;
}

CVCamFilter::CVCamFilter(LPUNKNOWN lpunk, HRESULT *phr) : CSource(NAME("Simple Virtual Cam"), lpunk, CLSID_VCamFilter) {
    ASSERT(phr);
    CAutoLock cAutoLock(&m_cStateLock);

    // Create the one and only output pin
    m_paStreams = (CSourceStream **) new CVCamOutStream *[1];
    m_paStreams[0] = new CVCamOutStream(phr, this, L"Simple Virtual Cam");
}

HRESULT CVCamFilter::QueryInterface(REFIID riid, void **ppv) {
    //Forward request for IAMStreamConfig & IKsPropertySet to the pin
    if (riid == _uuidof(IAMStreamConfig) || riid == _uuidof(IKsPropertySet))
        return m_paStreams[0]->QueryInterface(riid, ppv);
    else
        return CSource::QueryInterface(riid, ppv);
}
