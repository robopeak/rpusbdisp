# Introduction

RoboPeak Mini USB Display is a low cost display module with USB connectivity for
data transmission and designed by RoboPeak Team. It can be convenient used as a
Human Interactive Interface device for various embedded devices and platforms.

# Resources
We provides documents, precompiled ROMs and any related materials at the product
page of our document wiki:
http://www.robopeak.com/docs/doku.php?id=product-rpusbdisp

# User Mode SDK
User mode sdk is a set of libraries and APIs used to enable RoboPeak Mini USB
Display without flashing the firmware of your device or installing driver (Driver
is still needed by Windows platform).

If you are using display with Windows, OS X, please use this SDK.

If you don't or not be allowed to recompile the kernel or install kernel module, please use this SDK.

If you want more control over the display, please use this SDK.

# Build
We provided makefiles as well as IDE project files to easily build the SDK.

## Windows
Please ensure you have Visual Studio 2013 (or Express) installed. Just open
the project file located at workspaces/vs2013, and build like any other visual
studio projects.

After build please copy the libusb-1.0.dll from workspace/vs2013/Win32/[Debug|Release]/dll to workspace/vs2013/rpusbdisp_sdk/[Debug|Release]

On Windows, you need to install a libusb driver for RoboPeak Mini USB Display device.

The simplest way is use zadig's install which is located [here](http://zadig.zkeo.ie), you can use either libusb-win32 driver or libusbk driver, Microsoft WinUSB driver will not work.

## OS X (Mac OS X)
Just open the xcode project in workspaces/xcode with latest Xcode and build.

## Linux
Just run make in this directory, to build the SDK. We use some C++ 11 features
(mutex, thread, functional, memory, and etc.), so you may need GCC 4.8 to build
this SDK.

# APIs
Both C++ and C APIs are provided. You may choose to your own flavour. They provide
the same features.

## C++ APIs
### Headers
```c++
#include <rp/infra_config.h> // The configuration of infra
#include <rp/drivers/display/rpusbdisp/rpusbdisp.h> // The RoboPeak USB Display

// In order to simplify the sample, we use the following namesapces
using namespace std;
using namespace rp::deps::libusbx_wrap;
using namespace rp::drivers::display;
```

### Open Device
Devices need to be open before use.

#### Easy Way
You can just use a shortcut to open the first
available device connected to your computer (this is very useful if you only want
to use one display):
```c++
shared_ptr<RoboPeakUsbDisplayDevice> device = RoboPeakUsbDisplayDevice::openFirstDevice();
```

#### More Flexible Way
You can also enumerate devices and open device by yourself
```c++
// Enumerate devices
vector<shared_ptr<Device>> usbDevices = RoboPeakUsbDisplayDevice::enumDevices();

shared_ptr<Device> usbDevice = pick_device(usbDevices);

shared_ptr<RoboPeakUsbDisplayDevice> device = make_shared<RoboPeakUsbDisplayDevice>(usbDevice->openDevice());
```

### Device Operation APIs
Please refer to the rpusbdisp-drv/include/rp/drivers/display/rpusbdisp/rpusbdisp.h for all paint APIs provided by the SDK

## C APIs
### Headers
```c
#include <rp/infra_config.h>
#include <rp/drivers/display/rpusbdisp/c_interfaces.h>
```

The usage of C APIs is similar to the C++ API, please refer to the rpusbdisp-drv/include/rp/drivers/display/rpusbdisp/c_intefaces.h for detailed information

# Demo
A demo is included in this SDK to demonstrate the usage of the SDK which is organised in the demo directory
