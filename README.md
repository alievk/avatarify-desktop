# Avatarify Desktop

Notion: https://www.notion.so/54100a62141b4a0182df043a24b12d34?v=630854456ebb4f39bbf1faa43942742f

## Getting started
### Notes
* **CMake**. This is CMake project (nor Qt nor VS). CMake was chosen because libtorch recommends to use one. CMake is best with [CLion](https://www.jetbrains.com/clion/), but you are free to write a QMake build to use QT Creator.

* **VS compiler**. Precompiled libtorch libraries are built with VS compiler. Libtorch sources compile without errors with VS compiler, but mingw causes errors. It seems reasonable to buy license for VS compiler than make libtorch support for mingw.

* **AKVitrualCamera**. We use my fork of [AKVitrualCamera](https://github.com/vlivashkin/akvirtualcamera) to support vcam. It has GNUv3 license, can we use it? In the fork I've added CMake support for Windows (you'll need to add MacOS support). You also need to rename all stuff there (e.g. service name) to avoid collision with Webcamoid.

### Dependencies
* **libtorch**. Library for NN infrernce. Download library here: https://pytorch.org/ (Stable; Windows; C++/Java; 10.2/None). Direct links: if you have Nvidia GPU [release](https://download.pytorch.org/libtorch/cu102/libtorch-win-shared-with-deps-1.6.0.zip), [debug](https://download.pytorch.org/libtorch/cu102/libtorch-win-shared-with-deps-debug-1.6.0.zip), otherwise [release](https://download.pytorch.org/libtorch/cpu/libtorch-win-shared-with-deps-1.6.0%2Bcpu.zip), [debug](https://download.pytorch.org/libtorch/cpu/libtorch-win-shared-with-deps-debug-1.6.0%2Bcpu.zip). 

  Extract files to C:\\SDKs\\, exact naming in [CMakeLists.txt](CMakeLists.txt).
  
* **libyuv**. Library for fast frame format conversion, e.g. YUYV -> RGB. [Official manual](https://chromium.googlesource.com/libyuv/libyuv/+/HEAD/docs/getting_started.md).
 
  Short one:
  * Clone [depot tools](https://commondatastorage.googleapis.com/chrome-infra-docs/flat/depot_tools/docs/html/depot_tools_tutorial.html#_setting_up) to C:\SDK:
    ```bash
    git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
    ```
  * Add C:\\SDK\\depot_tools to PATH
  * Clone libyuv:
    ```bash
    gclient config --name src https://chromium.googlesource.com/libyuv/libyuv
    gclient sync
    ```
  * Building with ninja from manual fails. Just build all targets with CMake and CLion.
  
* **dlib**. Library for fast face tracking (needed by kalman crop). [Download here](https://github.com/davisking/dlib/releases/tag/v19.21), put to C:\\SDKs\\dlib-19.21 (see [CMakeLists.txt](CMakeLists.txt)), build all targets with CMake and CLion. Dlib has no way to be properly linked, so I had to use [crutch](https://stackoverflow.com/a/43821988/5438422): add all/sources.cpp from dlib to my CMake project.

* **eigen3**. Library for fast matrix computations (needed by kalman crop). [Download here](http://eigen.tuxfamily.org/index.php?title=Main_Page), put to C:\\SDKs\\eigen3 (see [CMakeLists.txt](CMakeLists.txt)). No need to compile, we will use only headers.

### Build project
```bash
git clone https://github.com/vlivashkin/avatarify-desktop-cpp
git submodule update --init
# in CLion: choose main CMakeLists.txt, target Avatarify, executable Avatarify
```