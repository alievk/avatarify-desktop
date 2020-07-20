@echo off
set scriptpath=%~dp0
net session >nul 2>&1
if %errorLevel% == 0 (
    echo Administrator PRIVILEGES Detected!
    REGSVR32 \u %scriptpath%..\cmake-build-release\DSVCam.dll
) else (
    echo NOT AN ADMIN!
)
pause >nul