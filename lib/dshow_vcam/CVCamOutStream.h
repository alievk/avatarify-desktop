#ifndef SVCAM_VCAMFILTERSTREAM_H
#define SVCAM_VCAMFILTERSTREAM_H

#include "stdafx.h"
#include "CVCamFilter.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

//------------------------------------------------------------------------------
// Class CVCamOutStream
//
// This class implements the stream which is used to output the bouncing ball
// data from the source filter. It inherits from DirectShows's base
// CSourceStream class.
//------------------------------------------------------------------------------
class CVCamOutStream : public CSourceStream, public IAMStreamConfig, public IKsPropertySet, public IAMFilterMiscFlags {
public:

    //////////////////////////////////////////////////////////////////////////
    //  IUnknown
    //////////////////////////////////////////////////////////////////////////
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv) override;

    STDMETHODIMP_(ULONG) AddRef() override;

    STDMETHODIMP_(ULONG) Release() override;

    //////////////////////////////////////////////////////////////////////////
    //  IQualityControl
    //////////////////////////////////////////////////////////////////////////
    STDMETHODIMP Notify(IBaseFilter *pSender, Quality q) override;

    //////////////////////////////////////////////////////////////////////////
    //  IAMStreamConfig
    //////////////////////////////////////////////////////////////////////////
    HRESULT STDMETHODCALLTYPE SetFormat(AM_MEDIA_TYPE *pmt) override;

    HRESULT STDMETHODCALLTYPE GetFormat(AM_MEDIA_TYPE **ppmt) override;

    HRESULT STDMETHODCALLTYPE GetNumberOfCapabilities(int *piCount, int *piSize) override;

    HRESULT STDMETHODCALLTYPE GetStreamCaps(int iIndex, AM_MEDIA_TYPE **pmt, BYTE *pSCC) override;

    //////////////////////////////////////////////////////////////////////////
    //  IKsPropertySet
    //////////////////////////////////////////////////////////////////////////
    HRESULT
    STDMETHODCALLTYPE Set(REFGUID guidPropSet, DWORD dwID, void *pInstanceData, DWORD cbInstanceData, void *pPropData,
                          DWORD cbPropData) override;

    HRESULT STDMETHODCALLTYPE Get(REFGUID guidPropSet, DWORD dwPropID, void *pInstanceData, DWORD cbInstanceData,
                                  void *pPropData, DWORD cbPropData, DWORD *pcbReturned) override;

    HRESULT STDMETHODCALLTYPE QuerySupported(REFGUID guidPropSet, DWORD dwPropID, DWORD *pTypeSupport) override;

    //////////////////////////////////////////////////////////////////////////
    //  IAMFilterMiscFlags
    //////////////////////////////////////////////////////////////////////////

    ULONG STDMETHODCALLTYPE GetMiscFlags() override;

    //////////////////////////////////////////////////////////////////////////
    //  CSourceStream
    //////////////////////////////////////////////////////////////////////////
    CVCamOutStream(HRESULT *phr, CVCamFilter *pParent, LPCWSTR pPinName);

    ~CVCamOutStream() override;

    HRESULT FillBuffer(IMediaSample *pms) override;

    HRESULT DecideBufferSize(IMemAllocator *pIMemAlloc, ALLOCATOR_PROPERTIES *pProperties) override;

    HRESULT CheckMediaType(const CMediaType *pMediaType) override;

    HRESULT GetMediaType(int iPosition, CMediaType *pmt) final;

    HRESULT SetMediaType(const CMediaType *pmt) override;

    HRESULT OnThreadCreate() override;

private:
    CVCamFilter *m_pParent;
    REFERENCE_TIME m_rtLastTime = 0;

    shared_memory_object shm;
};

#endif
