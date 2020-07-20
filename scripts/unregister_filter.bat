@echo off
set scriptpath=%~dp0
net session >nul 2>&1
if %errorLevel% == 0 (
    echo Administrator PRIVILEGES Detected!
    REGSVR32 /U %scriptpath%..\cmake-build-release\lib\ds_vcam\DSVCam.dll
) else (
    echo NOT AN ADMIN!
    pause >nul
)
