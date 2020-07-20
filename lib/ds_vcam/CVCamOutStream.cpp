#include "CVCamOutStream.h"


//////////////////////////////////////////////////////////////////////////
// CKCamStream is the one and only output pin of CKCam which handles
// all the stuff.
//////////////////////////////////////////////////////////////////////////
CVCamOutStream::CVCamOutStream(HRESULT *phr, CVCamFilter *pParent, LPCWSTR pPinName) :
        CSourceStream(NAME("Capture"), phr, pParent, pPinName), m_pParent(pParent) {
    // Set the default media type as 640x480x24@30
    GetMediaType(8, &m_mt);
}

CVCamOutStream::~CVCamOutStream() = default;

ULONG CVCamOutStream::Release() {
    return GetOwner()->Release();
}

ULONG CVCamOutStream::AddRef() {

    return GetOwner()->AddRef();
}

HRESULT CVCamOutStream::QueryInterface(REFIID riid, void **ppv) {

    // Standard OLE stuff
    if (riid == _uuidof(IAMStreamConfig))
        *ppv = (IAMStreamConfig *) this;
    else if (riid == _uuidof(IKsPropertySet))
        *ppv = (IKsPropertySet *) this;
    else
        return CSourceStream::QueryInterface(riid, ppv);

    AddRef();
    return S_OK;
}

///////////////////////////////////////////////////////////
// This is where the magic happens!
///////////////////////////////////////////////////////////
HRESULT CVCamOutStream::FillBuffer(IMediaSample *pms) {
    REFERENCE_TIME rtNow;
    REFERENCE_TIME avgFrameTime = ((VIDEOINFOHEADER *) m_mt.pbFormat)->AvgTimePerFrame;

    rtNow = m_rtLastTime;
    m_rtLastTime += avgFrameTime;
    pms->SetTime(&rtNow, &m_rtLastTime);
    pms->SetSyncPoint(TRUE);

    BYTE *pData;
    pms->GetPointer(&pData);
    long lDataLen = pms->GetSize();
    for (int i = 0; i < lDataLen; ++i)
        pData[i] = rand();

    return NOERROR;

}

//
// Notify
// Ignore quality management messages sent from the downstream filter
STDMETHODIMP CVCamOutStream::Notify(IBaseFilter *pSender, Quality q) {
    return E_NOTIMPL;
} // Notify


//////////////////////////////////////////////////////////////////////////
// This is called when the output format has been negotiated
//////////////////////////////////////////////////////////////////////////
HRESULT CVCamOutStream::SetMediaType(const CMediaType *pmt) {
    HRESULT hr = CSourceStream::SetMediaType(pmt);
    return hr;
}

// See Directshow help topic for IAMStreamConfig for details on this method
HRESULT CVCamOutStream::GetMediaType(int iPosition, CMediaType *pmt) {
    if (iPosition < 0) return E_INVALIDARG;
    if (iPosition > 8) return VFW_S_NO_MORE_ITEMS;

    if (iPosition == 0) {
        *pmt = m_mt;
        return S_OK;
    }

    auto *pvi = (VIDEOINFOHEADER *) pmt->AllocFormatBuffer(sizeof(VIDEOINFOHEADER));
    ZeroMemory(pvi, sizeof(VIDEOINFOHEADER));

    pvi->bmiHeader.biCompression = BI_RGB;
    pvi->bmiHeader.biBitCount = 24;
    pvi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pvi->bmiHeader.biWidth = 80 * iPosition;
    pvi->bmiHeader.biHeight = 60 * iPosition;
    pvi->bmiHeader.biPlanes = 1;
    pvi->bmiHeader.biSizeImage = GetBitmapSize(&pvi->bmiHeader);
    pvi->bmiHeader.biClrImportant = 0;

    pvi->AvgTimePerFrame = 1000000;

    SetRectEmpty(&(pvi->rcSource)); // we want the whole image area rendered.
    SetRectEmpty(&(pvi->rcTarget)); // no particular destination rectangle

    pmt->SetType(&MEDIATYPE_Video);
    pmt->SetFormatType(&FORMAT_VideoInfo);
    pmt->SetTemporalCompression(FALSE);

    // Work out the GUID for the subtype from the header info.
    const GUID SubTypeGUID = GetBitmapSubtype(&pvi->bmiHeader);
    pmt->SetSubtype(&SubTypeGUID);
    pmt->SetSampleSize(pvi->bmiHeader.biSizeImage);

    return NOERROR;

} // GetMediaType


// This method is called to see if a given output format is supported
HRESULT CVCamOutStream::CheckMediaType(const CMediaType *pMediaType) {
    if (*pMediaType != m_mt)
        return E_INVALIDARG;
    return S_OK;
} // CheckMediaType

// This method is called after the pins are connected to allocate buffers to stream data
HRESULT CVCamOutStream::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties) {
    CAutoLock cAutoLock(m_pFilter->pStateLock());
    HRESULT hr = NOERROR;

    auto *pvi = (VIDEOINFOHEADER *) m_mt.Format();
    pProperties->cBuffers = 1;
    pProperties->cbBuffer = pvi->bmiHeader.biSizeImage;

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties, &Actual);

    if (FAILED(hr)) return hr;
    if (Actual.cbBuffer < pProperties->cbBuffer) return E_FAIL;

    return NOERROR;
} // DecideBufferSize

// Called when graph is run
HRESULT CVCamOutStream::OnThreadCreate() {
    m_rtLastTime = 0;
    return NOERROR;
} // OnThreadCreate

//////////////////////////////////////////////////////////////////////////
//  IAMStreamConfig
//////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CVCamOutStream::SetFormat(AM_MEDIA_TYPE *pmt) {
    m_mt = *pmt;
    IPin *pin;
    ConnectedTo(&pin);
    if (pin) {
        IFilterGraph *pGraph = m_pParent->GetGraph();
        pGraph->Reconnect(this);
    }
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CVCamOutStream::GetFormat(AM_MEDIA_TYPE **ppmt) {
    *ppmt = CreateMediaType(&m_mt);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CVCamOutStream::GetNumberOfCapabilities(int *piCount, int *piSize) {
    *piCount = 8;
    *piSize = sizeof(VIDEO_STREAM_CONFIG_CAPS);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CVCamOutStream::GetStreamCaps(int iIndex, AM_MEDIA_TYPE **pmt, BYTE *pSCC) {
    *pmt = CreateMediaType(&m_mt);

    if (iIndex == 0) iIndex = 4;

    auto *pvi = (VIDEOINFOHEADER *) (*pmt)->pbFormat;
    pvi->bmiHeader.biCompression = BI_RGB;
    pvi->bmiHeader.biBitCount = 24;
    pvi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pvi->bmiHeader.biWidth = 80 * iIndex;
    pvi->bmiHeader.biHeight = 60 * iIndex;
    pvi->bmiHeader.biPlanes = 1;
    pvi->bmiHeader.biSizeImage = GetBitmapSize(&pvi->bmiHeader);
    pvi->bmiHeader.biClrImportant = 0;

    SetRectEmpty(&(pvi->rcSource)); // we want the whole image area rendered.
    SetRectEmpty(&(pvi->rcTarget)); // no particular destination rectangle

    (*pmt)->majortype = MEDIATYPE_Video;
    (*pmt)->subtype = MEDIASUBTYPE_RGB24;
    (*pmt)->formattype = FORMAT_VideoInfo;
    (*pmt)->bTemporalCompression = FALSE;
    (*pmt)->bFixedSizeSamples = FALSE;
    (*pmt)->lSampleSize = pvi->bmiHeader.biSizeImage;
    (*pmt)->cbFormat = sizeof(VIDEOINFOHEADER);

    auto *pvscc = (VIDEO_STREAM_CONFIG_CAPS *) pSCC;
    pvscc->guid = FORMAT_VideoInfo;
    pvscc->VideoStandard = AnalogVideo_None;
    pvscc->InputSize.cx = 640;
    pvscc->InputSize.cy = 480;
    pvscc->MinCroppingSize.cx = 80;
    pvscc->MinCroppingSize.cy = 60;
    pvscc->MaxCroppingSize.cx = 640;
    pvscc->MaxCroppingSize.cy = 480;
    pvscc->CropGranularityX = 80;
    pvscc->CropGranularityY = 60;
    pvscc->CropAlignX = 0;
    pvscc->CropAlignY = 0;

    pvscc->MinOutputSize.cx = 80;
    pvscc->MinOutputSize.cy = 60;
    pvscc->MaxOutputSize.cx = 640;
    pvscc->MaxOutputSize.cy = 480;
    pvscc->OutputGranularityX = 0;
    pvscc->OutputGranularityY = 0;
    pvscc->StretchTapsX = 0;
    pvscc->StretchTapsY = 0;
    pvscc->ShrinkTapsX = 0;
    pvscc->ShrinkTapsY = 0;
    pvscc->MinFrameInterval = 200000;   //50 fps
    pvscc->MaxFrameInterval = 50000000; // 0.2 fps
    pvscc->MinBitsPerSecond = (80 * 60 * 3 * 8) / 5;
    pvscc->MaxBitsPerSecond = 640 * 480 * 3 * 8 * 50;

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// IKsPropertySet
//////////////////////////////////////////////////////////////////////////


HRESULT CVCamOutStream::Set(REFGUID guidPropSet, DWORD dwID, void *pInstanceData,
                            DWORD cbInstanceData, void *pPropData,
                            DWORD cbPropData) {// Set: Cannot set any properties.
    return E_NOTIMPL;
}

// Get: Return the pin category (our only property).
HRESULT CVCamOutStream::Get(
        REFGUID guidPropSet,   // Which property set.
        DWORD dwPropID,        // Which property in that set.
        void *pInstanceData,   // Instance data (ignore).
        DWORD cbInstanceData,  // Size of the instance data (ignore).
        void *pPropData,       // Buffer to receive the property data.
        DWORD cbPropData,      // Size of the buffer.
        DWORD *pcbReturned     // Return the size of the property.
) {
    if (guidPropSet != AMPROPSETID_Pin) return E_PROP_SET_UNSUPPORTED;
    if (dwPropID != AMPROPERTY_PIN_CATEGORY) return E_PROP_ID_UNSUPPORTED;
    if (pPropData == nullptr && pcbReturned == nullptr) return E_POINTER;

    if (pcbReturned) *pcbReturned = sizeof(GUID);
    if (pPropData == nullptr) return S_OK; // Caller just wants to know the size.
    if (cbPropData < sizeof(GUID)) return E_UNEXPECTED;// The buffer is too small.

    *(GUID *) pPropData = PIN_CATEGORY_CAPTURE;
    return S_OK;
}

// QuerySupported: Query whether the pin supports the specified property.
HRESULT CVCamOutStream::QuerySupported(REFGUID guidPropSet, DWORD dwPropID, DWORD *pTypeSupport) {
    if (guidPropSet != AMPROPSETID_Pin) return E_PROP_SET_UNSUPPORTED;
    if (dwPropID != AMPROPERTY_PIN_CATEGORY) return E_PROP_ID_UNSUPPORTED;
    // We support getting this property, but not setting it.
    if (pTypeSupport) *pTypeSupport = KSPROPERTY_SUPPORT_GET;
    return S_OK;
}

// Set misc flag that this is truly a live source
ULONG CVCamOutStream::GetMiscFlags() {
    return AM_FILTER_MISC_FLAGS_IS_SOURCE;
}