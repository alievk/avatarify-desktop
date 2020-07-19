# svcam
This thing is a custom DirectShow filter.
* https://docs.microsoft.com/en-us/windows/win32/directshow/how-to-create-a-dll
* Good manual in README https://github.com/EnsignPayton/DshowTestFilters


### Register DLL (Needs administrator privileges)
* https://docs.microsoft.com/en-us/windows/win32/directshow/how-to-register-directshow-filters
* https://stackoverflow.com/questions/3560855/how-to-use-install-custom-directshow-filter
```bash
# register:
REGSVR32 ./DSVCam.dll
# unregister
REGSVR32 /u ./DSVCam.dll
```
* The error 0x80070005 means Permission Denied accessing the folders or files. 
* To check that it is registered: graphedt.exe (inside Windows SDK, e.g. C:\Program Files (x86)\Windows Kits\10\bin\10.0.18362.0\x64\graphedt.exe)

### Register in app
???

###