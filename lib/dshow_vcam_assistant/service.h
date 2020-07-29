#ifndef SERVICE_H
#define SERVICE_H

#include <minwindef.h>

namespace AkVCam
{
    class Service
    {
        public:
            Service();
            ~Service();

            BOOL install();
            void uninstall();
            void debug();
            void showHelp(int argc, char **argv);
    };
}

void WINAPI serviceMain(DWORD dwArgc, LPTSTR *lpszArgv);

#endif // SERVICE_H
