#include "stdafx.h"

#define DECLARE_PTR(type, ptr, expr) type* ptr = (type*)(expr);


EXTERN_C const GUID CLSID_SimpleVirtualCamFilter;


//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------
// The class managing the output pin
class CSimpleVirtualCamFilterStream;

//------------------------------------------------------------------------------
// Class CSimpleVirtualCamFilter
//
// This is the main class for the virtual cam filter. It inherits from
// CSource, the DirectShow base class for source filters.
//------------------------------------------------------------------------------
class CSimpleVirtualCamFilter : public CSource {
public:

    // The only allowed way to create Bouncing balls!
    static CUnknown *WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);

    IFilterGraph *GetGraph() { return m_pGraph; }

private:

    // It is only allowed to to create these objects with CreateInstance
    CSimpleVirtualCamFilter(LPUNKNOWN lpunk, HRESULT *phr);

}; // CSimpleVirtualCamFilter

//------------------------------------------------------------------------------
// Class CBallStream
//
// This class implements the stream which is used to output the bouncing ball
// data from the source filter. It inherits from DirectShows's base
// CSourceStream class.
//------------------------------------------------------------------------------
class CSimpleVirtualCamFilterStream
        : public CSourceStream, public IAMStreamConfig, public IKsPropertySet, public IAMFilterMiscFlags {

public:

    //////////////////////////////////////////////////////////////////////////
    //  IUnknown
    //////////////////////////////////////////////////////////////////////////
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);

    STDMETHODIMP_(ULONG) AddRef();

    STDMETHODIMP_(ULONG) Release();

    //////////////////////////////////////////////////////////////////////////
    //  IQualityControl
    //////////////////////////////////////////////////////////////////////////
    STDMETHODIMP Notify(IBaseFilter *pSender, Quality q);

    //////////////////////////////////////////////////////////////////////////
    //  IAMStreamConfig
    //////////////////////////////////////////////////////////////////////////
    HRESULT STDMETHODCALLTYPE SetFormat(AM_MEDIA_TYPE *pmt);

    HRESULT STDMETHODCALLTYPE GetFormat(AM_MEDIA_TYPE **ppmt);

    HRESULT STDMETHODCALLTYPE GetNumberOfCapabilities(int *piCount, int *piSize);

    HRESULT STDMETHODCALLTYPE GetStreamCaps(int iIndex, AM_MEDIA_TYPE **pmt, BYTE *pSCC);

    //////////////////////////////////////////////////////////////////////////
    //  IKsPropertySet
    //////////////////////////////////////////////////////////////////////////
    HRESULT
    STDMETHODCALLTYPE Set(REFGUID guidPropSet, DWORD dwID, void *pInstanceData, DWORD cbInstanceData, void *pPropData,
                          DWORD cbPropData);

    HRESULT STDMETHODCALLTYPE Get(REFGUID guidPropSet, DWORD dwPropID, void *pInstanceData, DWORD cbInstanceData,
                                  void *pPropData, DWORD cbPropData, DWORD *pcbReturned);

    HRESULT STDMETHODCALLTYPE QuerySupported(REFGUID guidPropSet, DWORD dwPropID, DWORD *pTypeSupport);

    //////////////////////////////////////////////////////////////////////////
    //  IAMFilterMiscFlags
    //////////////////////////////////////////////////////////////////////////

    ULONG STDMETHODCALLTYPE GetMiscFlags(void);

    //////////////////////////////////////////////////////////////////////////
    //  CSourceStream
    //////////////////////////////////////////////////////////////////////////
    CSimpleVirtualCamFilterStream(HRESULT *phr, CSimpleVirtualCamFilter *pParent, LPCWSTR pPinName);

    ~CSimpleVirtualCamFilterStream();

    HRESULT FillBuffer(IMediaSample *pms);

    HRESULT DecideBufferSize(IMemAllocator *pIMemAlloc, ALLOCATOR_PROPERTIES *pProperties);

    HRESULT CheckMediaType(const CMediaType *pMediaType);

    HRESULT GetMediaType(int iPosition, CMediaType *pmt);

    HRESULT SetMediaType(const CMediaType *pmt);

    HRESULT OnThreadCreate(void);

private:

    CSimpleVirtualCamFilter *m_pParent;
    REFERENCE_TIME m_rtLastTime;
    HBITMAP m_hLogoBmp;
    CCritSec m_cSharedState;
    IReferenceClock *m_pClock;

}; // CSimpleVirtualCamFilterStream




/*
// This class is exported from the SVCam.dll
class SVCAM_API CSVCam {
public:
	CSVCam(void);
	// TODO: add your methods here.
};

extern SVCAM_API int nSVCam;

SVCAM_API int fnSVCam(void);

*/
