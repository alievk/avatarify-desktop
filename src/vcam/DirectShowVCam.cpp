#include <winnt.h>
#include <strmif.h>
#include "DirectShowVCam.h"

DirectShowVCam::DirectShowVCam() {
//    CoGetClassObject(rclsid, dwClsContext, NULL, IID_IClassFactory, &pCF);
//    hresult = pCF->CreateInstance(pUnkOuter, riid, ppvObj)
//    pCF->Release();
}


void DirectShowVCam::present(const QImage &generatedFrame) {

}

HRESULT DirectShowVCam::OpenFile(PCWSTR pszFileName)
{
    IBaseFilter *pSource = NULL;

    // Create a new filter graph. (This also closes the old one, if any.)
    HRESULT hr = InitializeGraph();
    if (FAILED(hr))
    {
        goto done;
    }

    // Add the source filter to the graph.
    hr = m_pGraph->AddSourceFilter(pszFileName, NULL, &pSource);
    if (FAILED(hr))
    {
        goto done;
    }

    // Try to render the streams.
    hr = RenderStreams(pSource);

    done:
    if (FAILED(hr))
    {
        TearDownGraph();
    }
    SafeRelease(&pSource);
    return hr;
}