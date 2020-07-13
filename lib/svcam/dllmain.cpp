


#pragma comment(lib, "kernel32")
#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "advapi32")
#pragma comment(lib, "winmm")
#pragma comment(lib, "ole32")
#pragma comment(lib, "oleaut32")


#ifdef _DEBUG
#pragma comment(lib, "strmbasd")
#else
#pragma comment(lib, "strmbase")
#endif


#include "stdafx.h"
#include "SVCam.h"


#define CreateComObject(clsid, iid, var) CoCreateInstance( clsid, NULL, CLSCTX_INPROC_SERVER, iid, (void **)&var);

STDAPI AMovieSetupRegisterServer(CLSID clsServer, LPCWSTR szDescription, LPCWSTR szFileName,
                                 LPCWSTR szThreadingModel = L"Both", LPCWSTR szServerType = L"InprocServer32");
STDAPI AMovieSetupUnregisterServer(CLSID clsServer);


// {D2205858-A855-4118-A724-A13AED4C61B2}
DEFINE_GUID(CLSID_SimpleVirtualCamFilter,
            0xd2205858, 0xa855, 0x4118, 0xa7, 0x24, 0xa1, 0x3a, 0xed, 0x4c, 0x61, 0xb2);


// Setup data

const AMOVIESETUP_MEDIATYPE sudOpPinTypes =
        {
                &MEDIATYPE_Video,       // Major type
                &MEDIASUBTYPE_RGB24      // Minor type
        };

const AMOVIESETUP_PIN sudOpPin[] =  // TODO Maybe the array is not a good idea?!
        {{
                 L"Output",              // Pin string name
                 FALSE,                  // Is it rendered
                 TRUE,                   // Is it an output
                 FALSE,                  // Can we have none
                 FALSE,                  // Can we have many
                 &CLSID_NULL,            // Connects to filter
                 NULL,                   // Connects to pin
                 1,                      // Number of types
                 &sudOpPinTypes}        // Pin details
        };

const AMOVIESETUP_FILTER sudSVCFilterAx =
        {
                &CLSID_SimpleVirtualCamFilter,    // Filter CLSID
                L"Simple Virtual Cam",       // String name
                MERIT_NORMAL,       // Filter merit
                1,                      // Number pins
                &sudOpPin[0]               // Pin details
        };

// COM global table of objects in this dll

CFactoryTemplate g_Templates[] = {
        {L"Simple Virtual Cam", &CLSID_SimpleVirtualCamFilter, CSimpleVirtualCamFilter::CreateInstance, NULL, &sudSVCFilterAx}
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


/*
STDAPI RegisterFilters(BOOL bRegister)
{
	HRESULT hr = NOERROR;
	WCHAR achFileName[MAX_PATH];
	char achTemp[MAX_PATH];
	ASSERT(g_hInst != 0);

	if (0 == GetModuleFileNameA(g_hInst, achTemp, sizeof(achTemp)))
		return AmHresultFromWin32(GetLastError());

	MultiByteToWideChar(CP_ACP, 0L, achTemp, lstrlenA(achTemp) + 1,
		achFileName, NUMELMS(achFileName));

	hr = CoInitialize(0);
	if (bRegister)
	{
		hr = AMovieSetupRegisterServer(CLSID_SimpleVirtualCamFilter, L"Simple Virtual Cam", achFileName, L"Both", L"InprocServer32");
	}

	if (SUCCEEDED(hr))
	{
		IFilterMapper2 *fm = 0;
		hr = CreateComObject(CLSID_FilterMapper2, IID_IFilterMapper2, fm);
		if (SUCCEEDED(hr))
		{
			if (bRegister)
			{
				IMoniker *pMoniker = 0;
				REGFILTER2 rf2;
				rf2.dwVersion = 1;
				rf2.dwMerit = MERIT_NORMAL;
				rf2.cPins = 1;
				rf2.rgPins = sudOpPin;
				hr = fm->RegisterFilter(CLSID_SimpleVirtualCamFilter, L"Simple Virtual Cam", &pMoniker, &CLSID_VideoInputDeviceCategory, NULL, &rf2);
			}
			else
			{
				hr = fm->UnregisterFilter(&CLSID_VideoInputDeviceCategory, 0, CLSID_SimpleVirtualCamFilter);
			}
		}

		// release interface
		//
		if (fm)
			fm->Release();
	}

	if (SUCCEEDED(hr) && !bRegister)
		hr = AMovieSetupUnregisterServer(CLSID_SimpleVirtualCamFilter);

	CoFreeUnusedLibraries();
	CoUninitialize();
	return hr;
}*/

//
// DllRegisterServer
//
// Exported entry points for registration and unregistration
//
STDAPI DllRegisterServer() {
    HRESULT hr;
    IFilterMapper2 *pFM2 = NULL;

    hr = AMovieDllRegisterServer2(TRUE);
    if (FAILED(hr))
        return hr;

    hr = CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER,
                          IID_IFilterMapper2, (void **) &pFM2);

    if (FAILED(hr))
        return hr;

    hr = pFM2->RegisterFilter(
            CLSID_SimpleVirtualCamFilter,                // Filter CLSID.
            L"Simple Virtual Cam",                       // Filter name.
            NULL,                            // Device moniker.
            &CLSID_VideoInputDeviceCategory,  // Video compressor category.
            L"Simple Virtual Cam",                       // Instance data.
            &rf2FilterReg                    // Pointer to filter information.
    );

    if (pFM2)
        pFM2->Release();

    return hr;

} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer() {
    HRESULT hr;
    IFilterMapper2 *pFM2 = NULL;

    hr = AMovieDllRegisterServer2(FALSE);
    if (FAILED(hr))
        return hr;

    hr = CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER,
                          IID_IFilterMapper2, (void **) &pFM2);

    if (FAILED(hr))
        return hr;

    hr = pFM2->UnregisterFilter(&CLSID_VideoInputDeviceCategory,
                                L"Simple Virtual Cam", CLSID_SimpleVirtualCamFilter);

    pFM2->Release();
    return hr;

} // DllUnregisterServer


//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
) {
    return DllEntryPoint((HINSTANCE) (hModule), ul_reason_for_call, lpReserved);
}

