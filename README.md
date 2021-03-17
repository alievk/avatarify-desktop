<img src=https://openavatarify.s3.amazonaws.com/media/einstein_flexin.gif width=600>

# Avatarify Desktop

Avatarify for desktop with a user friendly interface.

## System requirements
* Windows 10
* Nvidia GPU: GeForce 1070 or higher is recommended
* Web camera

## Installation

1. Download [Avatarify Desktop](https://avatarify-desktop.s3.amazonaws.com/installer/Avatarify+Desktop+v0.10.exe) (897 Mb).
1. Double click on the installer. Windows Defender warning screen may pop up telling that the app comes from an unknown source. Click "More info", then "Run anyway".
1. Follow the instructions of the setup Wizard. You may opt to the default settings.

## Running

1. The app starts automatically after the installation. If that didn't happen, press Windows button and start typing "Avatarify" and select "Avatarify Desktop". Windows Defender warning screen may pop up telling that the app comes from an unknown source. Click "More info", then "Run anyway".
1. In the **Settings** section of the main window, choose your web camera in the drop-down menu.
1. Position your head in the center of the frame and press C to calibrate.
1. Left-click on any avatar and start driving.

**Tip**: whenever your avatar screws, press C (or click **Calibrate** button) to reset the initial pose. 

To break into video conferencing with an avatar, set up the **Virtual camera**.

### Virtual camera

This example is for Zoom, but the procedure is similar in the other video-conf apps.

1. In Zoom, go to **Setting**s -> **Video**.
1. In the **Camera** drop-down menu, choose *Avatarify Camera*.
1. You should see your avatar immediately. If you see image from your web camera, go back to Avatarify and left-click on an avatar.

### Add new avatar

1. Click on **Open image file** in the main window.
1. Browse to your image with an avatar and open it. **Requirements to images**: one face, neutral emotion, frontal look.
1. Crop your image. Using the slider under the window, zoom image so that shoulder are slightly captured. Drag the image using left mouse button and center the head of the avatar. Leave some space between top of the head and image border.
1. Click on **Save** button. The new avatar will appear on the left of the avatar reel.

<img src=https://user-images.githubusercontent.com/4682236/101614195-b16b6480-3a1d-11eb-967d-465f3028b94d.PNG width=250>

### Use overlay

Avatar driving works better when your calibration pose matches the avatar's pose.

1. If an avatar is selected, left-click on it to switch to the web camera mode.
1. Right-click on the avatar to make it appear as an overlay on your web camera image.
1. Use **Overlay** slider in the **Settings** section to change opacity of the overlay.
1. Match your pose and expression with the avatar and press C to calibrate.
1. Left-click on the avatar to drive it.

<img src=https://user-images.githubusercontent.com/4682236/101615548-4fabfa00-3a1f-11eb-8258-c6d4dca85034.png width=400>

### Smart crop

The app has a function to automatically track the position of your head. It's useful when your web camera is mounted sidewise and you not appear in the center of the frame.

In the **Settings** section, switch **Smart crop** slider. The app will start tracking your head and move it to the center of the frame. When it's done, press C to calibrate.

## Controls

Key | Action
--- | ---
C | Calibrate. 
⬅️ ➡️| Switch to previous / next avatar.
F | Fullscreen mode



## Getting started
<!--
### Notes
* **VS compiler**. Precompiled libtorch libraries are built with VS compiler. Libtorch sources compile without errors with VS compiler, but mingw causes errors. It seems reasonable to buy a license for VS compiler than make libtorch support for mingw.

* **AKVitrualCamera**. We use my fork of [AKVitrualCamera](https://github.com/vlivashkin/akvirtualcamera) to support vcam. It has GNUv3 license, can we use it? In the fork, I've added CMake support for Windows (you'll need to add MacOS support). You also need to rename all stuff there (e.g. service name) to avoid collision with Webcamoid.
-->

### Dependencies
* **Qt**
1. Download [online installer](https://www.qt.io/download-open-source?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5)
1. Run the installer and on the Select Components page select these components:
    * Qt -> Qt 5.15.2. Deselect all components except MSVC 2019 64bit to save disk space
    * Qt -> Developer and Designer Tools -> OpenSSL 1.1.1d Toolkit

* **libtorch**. Library for NN inference. Download library here: https://pytorch.org/ (Stable; Windows; C++/Java; 10.2/None). Direct links: [release](https://download.pytorch.org/libtorch/cu102/libtorch-win-shared-with-deps-1.6.0.zip), [debug](https://download.pytorch.org/libtorch/cu102/libtorch-win-shared-with-deps-debug-1.6.0.zip). Extract files to
    * Release: C:\\SDKs\\libtorch-win-shared-with-deps-1.6.0
    * Debug: C:\\SDKs\\libtorch-win-shared-with-deps-debug-1.6.0  
 Make sure path C:\\SDKs\\libtorch-win-shared-with-deps-1.6.0\lib (for release) is correct. Remove extra sub-directories if necessary.  

<!--
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
-->

### Build project
* Check that you have libtorch at C:\\SDKs\\
```
git clone https://github.com/vlivashkin/avatarify-desktop
cd avatarify-desktop
git submodule update --init --recursive
```
* Build the project with Qt Creator. Choose MSVC 2019 64-bit kit and Release configuration.
* Download additional files from [here](https://avatarify-desktop.s3.amazonaws.com/.avatarify.zip) and unpack to C:\\Users\\[user]\\.avatarify. This folder has the following structure:
  * .avatarify\avatars: avatar pictures
  * .avatarify\models: FOMM and landmark detector model weights
* Download AkVirtualCamera.plugin from [here](https://avatarify-desktop.s3.amazonaws.com/AkVirtualCamera.plugin.zip) and unpack to [build]/avatarify. Avatarify will register еру plugin on the first run.

## How to build installer
(not very clean solution, but at least it works)

* Build release solution
* Call `make install` in cmake-build-release folder. Clean installation files will be located in C:\\Program Files\\Avatarify
* Compress folder with 7z and put it in avatarify-desktop\\installer\\packages\\com.avatarify.desktop\\data folder
* You need to make similar archives for models and avatars from C:\\Users\\[user]\\.avatarify and put them to com.avatarify.desktop.models and com.avatarify.desktop.avatars folders
* Run avatarify-desktop\\installer\\package_for_windows.bat
