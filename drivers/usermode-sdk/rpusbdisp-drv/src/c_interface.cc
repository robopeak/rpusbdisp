//
//  c_interface.cc
//  C Interface to RoboPeak Mini USB Display driver
//
//  Created by Tony Huang on 12/14/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <rp/drivers/display/rpusbdisp/c_interface.h>
#include <rp/deps/libusbx_wrap/libusbx_wrap.h>
#include <rp/drivers/display/rpusbdisp/rpusbdisp.h>
#include <functional>

using namespace std;
using namespace std::placeholders;
using namespace rp::util;
using namespace rp::drivers::display;
using namespace rp::deps::libusbx_wrap;

static inline shared_ptr<RoboPeakUsbDisplayDevice>* castDevice(RoboPeakUsbDisplayDeviceRef ref) {
    return reinterpret_cast<shared_ptr<RoboPeakUsbDisplayDevice>*>(ref);
}

static inline shared_ptr<RoboPeakUsbDisplayDevice>& getDevice(RoboPeakUsbDisplayDeviceRef ref) {
    return *castDevice(ref);
}

#define RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN try {
#define RPUSBDISP_HANDLE_EXCEPTIONS_END \
        return 0; \
    } catch(Exception& e) { \
        auto result = e.errorCode(); \
        return result; \
    }

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayOpenFirstDevice(RoboPeakUsbDisplayDeviceRef* outDevice) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        shared_ptr<RoboPeakUsbDisplayDevice> device = RoboPeakUsbDisplayDevice::openFirstDevice();
        
        if (!device) {
            *outDevice = 0;
            return -1;
        }
        
        *outDevice = reinterpret_cast<RoboPeakUsbDisplayDeviceRef>(new shared_ptr<RoboPeakUsbDisplayDevice>(device));
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayDisposeDevice(RoboPeakUsbDisplayDeviceRef device) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        delete castDevice(device);
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

static void status_callback_wrap(RoboPeakUsbDisplayStatusCallback callback, void* closure, const rpusbdisp_status_normal_packet_t& packet) {
    rpusbdisp_status_normal_packet_t* outPacket = const_cast<rpusbdisp_status_normal_packet_t*>(&packet);
    
    callback(outPacket, closure);
}

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplaySetStatusUpdatedCallback(RoboPeakUsbDisplayDeviceRef device, RoboPeakUsbDisplayStatusCallback callback, void* closure) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        getDevice(device)->setStatusUpdatedCallback(bind(status_callback_wrap, callback, closure, _1));
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayFill(RoboPeakUsbDisplayDeviceRef device, uint16_t color) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        getDevice(device)->fill(color);
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayBitblt(RoboPeakUsbDisplayDeviceRef device, uint16_t x, uint16_t y, uint16_t width, uint16_t height, RoboPeakUsbDisplayBitOperation bitOperation, void* buffer) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        getDevice(device)->bitblt(x, y, width, height, bitOperation, buffer);
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayFillRect(RoboPeakUsbDisplayDeviceRef device, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t color, RoboPeakUsbDisplayBitOperation bitOperation) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        getDevice(device)->fillrect(left, top, right, bottom, color, bitOperation);
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayCopyArea(RoboPeakUsbDisplayDeviceRef device, uint16_t srcX, uint16_t srcY, uint16_t destX, uint16_t destY, uint16_t width, uint16_t height) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        getDevice(device)->copyArea(srcX, srcY, destX, destY, width, height);
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayEnable(RoboPeakUsbDisplayDeviceRef device) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        getDevice(device)->enable();
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayGetResolution(RoboPeakUsbDisplayDeviceRef device, int* outWidth, int* outHeight) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        *outWidth = getDevice(device)->getWidth();
        *outHeight = getDevice(device)->getHeight();
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayIsAlive(RoboPeakUsbDisplayDeviceRef device, bool* outAlive) {
    RPUSBDISP_HANDLE_EXCEPTIONS_BEGIN
        *outAlive = getDevice(device)->isAlive();
    RPUSBDISP_HANDLE_EXCEPTIONS_END
}

