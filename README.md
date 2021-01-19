# Avatarify Desktop

Successor of [Open Avatarify](https://github.com/alievk/avatarify), written in C++.

Tasks in Notion: https://www.notion.so/54100a62141b4a0182df043a24b12d34?v=630854456ebb4f39bbf1faa43942742f.


## Getting started
### Notes
* **VS compiler**. Precompiled libtorch libraries are built with VS compiler. Libtorch sources compile without errors with VS compiler, but mingw causes errors. It seems reasonable to buy a license for VS compiler than make libtorch support for mingw.

* **AKVitrualCamera**. We use my fork of [AKVitrualCamera](https://github.com/vlivashkin/akvirtualcamera) to support vcam. It has GNUv3 license, can we use it? In the fork, I've added CMake support for Windows (you'll need to add MacOS support). You also need to rename all stuff there (e.g. service name) to avoid collision with Webcamoid.


### Dependencies
* **libtorch**. Library for NN inference. Download library here: https://pytorch.org/ (Stable; Windows; C++/Java; 10.2/None). Direct links: if you have Nvidia GPU [release](https://download.pytorch.org/libtorch/cu102/libtorch-win-shared-with-deps-1.6.0.zip), [debug](https://download.pytorch.org/libtorch/cu102/libtorch-win-shared-with-deps-debug-1.6.0.zip), otherwise [release](https://download.pytorch.org/libtorch/cpu/libtorch-win-shared-with-deps-1.6.0%2Bcpu.zip), [debug](https://download.pytorch.org/libtorch/cpu/libtorch-win-shared-with-deps-debug-1.6.0%2Bcpu.zip). 

  Extract files to C:\\SDKs\\.
  
* **libyuv**. Library for fast frame format conversion, e.g. YUYV -> RGB. [Official manual](https://chromium.googlesource.com/libyuv/libyuv/+/HEAD/docs/getting_started.md).
  
* **dlib**. Library for fast face tracking (needed by kalman crop). [Link](https://github.com/davisking/dlib/releases/tag/v19.21).

* **eigen3**. Library for fast matrix computations (needed by kalman crop). [Link](http://eigen.tuxfamily.org/index.php?title=Main_Page).

* **OpenSSL**. StackOverflow answer:
	```
	On Windows if you install Qt via online installer, you can select OpenSSL Toolkit as an optional component.
	This should provide you with the version definitely compatible with your Qt.
	If you haven't checked it during installation, you can rerun C:\Qt\MaintenanceTool.exe and select Add or remove components.
	OpenSSL Toolkit is located under the Developer and Designer Tools section, it is the last entry.
	However, there's a note in the sidebar:
	Qt installer doesn't set up OpenSSL environment. User needs to define the path and environment variables.
	So once installed, you should add the directory with the DLLs to the PATH environment variable or place the needed DLLs near the .exe file (copy them to the debug and release directories of your project).
	The directory is located at C:\Qt\Tools\OpenSSL\Win_x64\bin (or Win_x86 if you need 32-bit version). The DLLs are named libssl-1_1-x64.dll and libcrypto-1_1-x64.dll in my case.
	But this wasn't enough to deploy it to another machine, after copying it all I got the same error again.
	The reason came out to be that OpenSSL DLLs depend on another MSVC runtime version than the Qt app itself, and these errors go unreported.
	Open these DLLs in Dependency Walker to find out what's missing. In my case it was MSVCR100.dll, so I had to install MSVC 2010 Redistributable x64 (x86 is here).
	```


### Build project
* Check that you have libtorch at C:\\SDKs\\
```
git clone https://github.com/vlivashkin/avatarify-desktop
cd avatarify-desktop
git submodule update --init --recursive
```
* Build the project with Qt Creator
* Check that you have all additional files in ~/.avatarify (you can download dump [here](https://www.dropbox.com/s/c98y8b1bfijt1lm/.avatarify.zip?dl=0)):
  * ~/.avatarify/avatars:
    * _add.png (crutch for adding custom avatars)
    * some extra faces in 256x256
  * ~/.avatarify/models:
    * FOMMEncoder.pt_, KPDetector.pt_, FOMMNoEncoderNoKPDetector.pt_
    * shape_predictor_5_face_landmarks.dat
* Copy AkVirtualCamera.plugin folder to [build]/avatarify
* Copy libssl-1_1-x64.dll and libcrypto-1_1-x64.dll to [build]/avatarify/release

## How to build installer
(not very clean solution, but at least it works)

* Build release solution
* Call `make install` in cmake-build-release folder. Clean installation files will be located in C:\\Program Files\\Avatarify
* Compress folder with 7z and put it in ./installer/packages/com.avatarify.desktop/data folder
* You need to make similar archives for models and avatars from ~/.avatarify and put them to com.avatarify.desktop.models and com.avatarify.desktop.avatars folders
* Run ./installer/package_for_windows.bat