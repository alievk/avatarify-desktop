#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <Windows.h>
#include <shellapi.h>
#include <sddl.h>

#include "service.h"

BOOL AkVCam::Service::install()
{
    AkServiceLogMethod();
    WCHAR fileName[MAX_PATH];

    if (!GetModuleFileName(nullptr, fileName, MAX_PATH)) {
        AkLoggerLog("Can't read module file name");

       return false;
    }

    auto scManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);

    if (!scManager) {
        AkLoggerLog("Can't open SCManager");

        return false;
    }

    auto service =
            CreateService(scManager,
                          TEXT(DSHOW_PLUGIN_ASSISTANT_NAME),
                          TEXT(DSHOW_PLUGIN_ASSISTANT_DESCRIPTION),
                          SERVICE_ALL_ACCESS,
                          SERVICE_WIN32_OWN_PROCESS,
                          SERVICE_AUTO_START,
                          SERVICE_ERROR_NORMAL,
                          fileName,
                          nullptr,
                          nullptr,
                          nullptr,
                          nullptr,
                          nullptr);

    if (!service) {
        AkLoggerLog("Can't create service");
        CloseServiceHandle(scManager);

        return false;
    }

    // Add detailed description to the service.
    SERVICE_DESCRIPTION serviceDescription;
    WCHAR description[] = TEXT(DSHOW_PLUGIN_DESCRIPTION_EXT);
    serviceDescription.lpDescription = description;
    auto result =
            ChangeServiceConfig2(service,
                                 SERVICE_CONFIG_DESCRIPTION,
                                 &serviceDescription);

    // Configure the service so it will restart if fail.
    WCHAR rebootMsg[] = L"Service failed restarting...";

    std::vector<SC_ACTION> actions {
        {SC_ACTION_RESTART, 5000}
    };

    SERVICE_FAILURE_ACTIONS failureActions {
        INFINITE,
        rebootMsg,
        nullptr,
        DWORD(actions.size()),
        actions.data()
    };

    result =
        ChangeServiceConfig2(service,
                             SERVICE_CONFIG_FAILURE_ACTIONS,
                             &failureActions);

    // Run the service
    StartService(service, 0, nullptr);
    CloseServiceHandle(service);
    CloseServiceHandle(scManager);

    return result;
}

void AkVCam::Service::uninstall()
{
    AkServiceLogMethod();
    auto scManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);

    if (!scManager) {
        AkLoggerLog("Can't open SCManager");

        return;
    }

    auto sevice = OpenService(scManager,
                              TEXT(DSHOW_PLUGIN_ASSISTANT_NAME),
                              SERVICE_ALL_ACCESS);

    if (sevice) {
        if (ControlService(sevice,
                           SERVICE_CONTROL_STOP,
                           &servicePrivate()->m_status)) {
            AkLoggerLog("Stopping service");

            do {
                Sleep(1000);
                QueryServiceStatus(sevice, &servicePrivate()->m_status);
            } while(servicePrivate()->m_status.dwCurrentState != SERVICE_STOPPED);
        }

        if (!DeleteService(sevice)) {
            AkLoggerLog("Delete service failed");
        }

        CloseServiceHandle(sevice);
    } else {
        AkLoggerLog("Can't open service");
    }

    CloseServiceHandle(scManager);
}
