#pragma comment(lib, "kernel32")
#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "advapi32")
#pragma comment(lib, "winmm")
#pragma comment(lib, "ole32")
#pragma comment(lib, "oleaut32")

#if _MSC_VER >= 1900
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "strmbasd")
#else
#pragma comment(lib, "strmbase")
#endif

#include "stdafx.h"
#include "CVCamFilter.h"
#include "utils/Log.h"


#define CreateComObject(clsid, iid, var) CoCreateInstance( clsid, NULL, CLSCTX_INPROC_SERVER, iid, (void **)&var);

STDAPI AMovieSetupRegisterServer(CLSID clsServer, LPCWSTR szDescription, LPCWSTR szFileName,
                                 LPCWSTR szThreadingModel = L"Both", LPCWSTR szServerType = L"InprocServer32");
STDAPI AMovieSetupUnregisterServer(CLSID clsServer);


// {D2205858-A855-4118-A724-A13AED4C61B4}
DEFINE_GUID(CLSID_VCamFilter,
            0xd2205858, 0xa855, 0x4118, 0xa7, 0x24, 0xa1, 0x3a, 0xed, 0x4c, 0x61, 0xb4);


// Setup data

const AMOVIESETUP_MEDIATYPE sudOpPinTypes = {
        &MEDIATYPE_Video,       // Major type
        &MEDIASUBTYPE_RGB24      // Minor type
};

const AMOVIESETUP_PIN sudOpPin[] = { // TODO Maybe the array is not a good idea?!
        {
                L"Output",              // Pin string name
                FALSE,                  // Is it rendered
                TRUE,                   // Is it an output
                FALSE,                  // Can we have none
                FALSE,                  // Can we have many
                &CLSID_NULL,            // Connects to filter
                nullptr,                   // Connects to pin
                1,                      // Number of types
                &sudOpPinTypes
        }        // Pin details
};

const AMOVIESETUP_FILTER sudSVCFilterAx = {
        &CLSID_VCamFilter,    // Filter CLSID
        L"Avatarify DShow Camera",       // String name
        MERIT_NORMAL,       // Filter merit
        1,                      // Number pins
        &sudOpPin[0]               // Pin details
};

// COM global table of objects in this dll

CFactoryTemplate g_Templates[] = {
        {
                L"Avatarify DShow Camera",
                &CLSID_VCamFilter,
                CVCamFilter::CreateInstance,
                nullptr,
                &sudSVCFilterAx
        }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


////////////////////////////////////////////////////////////////////////
//
// Exported entry points for registration and unregistration 
// (in this case they only call through to default implementations).
//
////////////////////////////////////////////////////////////////////////


REGFILTER2 rf2FilterReg = {
        1,              // Version 1 (no pin mediums or pin category).
        MERIT_NORMAL,   // Merit.
        1,              // Number of pins.
        &sudOpPin[0]        // Pointer to pin information.
};


//
// DllRegisterServer
//
// Exported entry points for registration and unregistration
//
STDAPI DllRegisterServer() {
    HRESULT hr;
    IFilterMapper2 *pFM2 = nullptr;

    hr = AMovieDllRegisterServer2(TRUE);
    if (FAILED(hr))
        return hr;

    hr = CoCreateInstance(CLSID_FilterMapper2, nullptr, CLSCTX_INPROC_SERVER,
                          IID_IFilterMapper2, (void **) &pFM2);

    if (FAILED(hr))
        return hr;

    hr = pFM2->RegisterFilter(
            CLSID_VCamFilter,                // Filter CLSID.
            L"Avatarify DShow Camera",                       // Filter name.
            nullptr,                            // Device moniker.
            &CLSID_VideoInputDeviceCategory,  // Video compressor category.
            L"Avatarify DShow Camera",                       // Instance data.
            &rf2FilterReg                    // Pointer to filter information.
    );

    if (pFM2)
        pFM2->Release();

    initLogger( R"(C:\Users\illus\.avatarify\vcam.log)", ldebug);
    L_(linfo) << "Filter registered!";

    return hr;
}


//
// DllUnregisterServer
//

STDAPI DllUnregisterServer() {
    HRESULT hr;
    IFilterMapper2 *pFM2 = nullptr;

    hr = AMovieDllRegisterServer2(FALSE);
    if (FAILED(hr))
        return hr;

    hr = CoCreateInstance(CLSID_FilterMapper2, nullptr, CLSCTX_INPROC_SERVER,
                          IID_IFilterMapper2, (void **) &pFM2);

    if (FAILED(hr))
        return hr;

    hr = pFM2->UnregisterFilter(&CLSID_VideoInputDeviceCategory, L"Avatarify DShow Camera", CLSID_VCamFilter);

    pFM2->Release();

    initLogger( R"(C:\Users\illus\.avatarify\vcam.log)", ldebug);
    L_(linfo) << "Filter unregistered!";

    return hr;
}


//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    initLogger( R"(C:\Users\illus\.avatarify\vcam.log)", ldebug);
    L_(linfo) << "Entrypoint!";

    return DllEntryPoint((HINSTANCE) (hModule), ul_reason_for_call, lpReserved);
}

