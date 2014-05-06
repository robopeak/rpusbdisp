//
//  main.cc
//  RoboPeak Mini USB Display SDK demo
//
//  Created by Tony Huang on 12/7/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <rp/infra_config.h>
#include <memory>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#ifdef RP_INFRA_PLATFORM_WINDOWS
#	include <io.h>
#	include <process.h>
#else
#	include <unistd.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <rp/infra_config.h>
#include <rp/deps/libusbx_wrap/libusbx_wrap.h>
#include <rp/drivers/display/rpusbdisp/rpusbdisp.h>
#include <rp/drivers/display/rpusbdisp/c_interface.h>

#undef USE_C_INTERFACE

#ifndef USE_C_INTERFACE
using namespace std;
using namespace rp::util;
using namespace rp::deps::libusbx_wrap;
using namespace rp::drivers::display;

static void onStatusUpdated(const rpusbdisp_status_normal_packet_t& status) {
    printf("Status: %02X, Touch: %02X, X: %d, Y: %d\n", status.display_status, status.touch_status, status.touch_x, status.touch_y);
}

static int cPlusPlusInterfaceDemo(void* framebuffer) {
    try {
        shared_ptr<RoboPeakUsbDisplayDevice> display = RoboPeakUsbDisplayDevice::openFirstDevice();
        
        if (!display) {
            fprintf(stderr, "No display found\n");
            return -1;
        }
        
        printf("Display with S/N %s is chosen\n", display->getDevice()->getSerialNumber().c_str());
        
        
        display->setStatusUpdatedCallback(onStatusUpdated);
        display->enable();
        
        this_thread::sleep_for(chrono::seconds(2));
        
        while (display->isAlive()) {
            display->bitblt(0, 0, 320, 240, RoboPeakUsbDisplayBitOperationCopy, framebuffer);
            this_thread::sleep_for(chrono::seconds(2));
            
            for (int i = 0; i < 100; i++) {
                uint16_t x = rand()%320;
                uint16_t y = rand()%240;
                uint16_t width = 1+(rand()%320);
                uint16_t height = 1+(rand()%240);
                uint16_t color = rand()&0xffffu;
                RoboPeakUsbDisplayBitOperation bitOperation = (RoboPeakUsbDisplayBitOperation)(rand()%4);
                
                display->fillrect(x, y, x + width, y + height, color, bitOperation);
            }
            this_thread::sleep_for(chrono::seconds(2));
            
            display->copyArea(0, 0, 160, 120, 160, 120);
            this_thread::sleep_for(chrono::seconds(2));
            
            display->fill(0xcb20u);
            this_thread::sleep_for(chrono::seconds(2));
        }
        
        fprintf(stderr, "Display is disconnected\n");
    } catch (Exception& e) {
        e.printToConsole();
        return e.errorCode();
    }

    return 0;
}

#else

static void cInterfaceStatusUpdatedCallback(rpusbdisp_status_normal_packet_t* status, void* closure) {
    printf("Status: %02X, Touch: %02X, X: %d, Y: %d\n", status->display_status, status->touch_status, status->touch_x, status->touch_y);
}

static int cInterfaceDemo(void* framebuffer) {
    RoboPeakUsbDisplayDeviceRef device;
    
    if (!RoboPeakUsbDisplayDriverIsSuccess(RoboPeakUsbDisplayOpenFirstDevice(&device))) {
        return -1;
    }
    
    if (!device) {
        fprintf(stderr, "No display found\n");
        return -1;
    }
    
    if (!RoboPeakUsbDisplayDriverIsSuccess(RoboPeakUsbDisplaySetStatusUpdatedCallback(device, cInterfaceStatusUpdatedCallback, 0))) {
        RoboPeakUsbDisplayDisposeDevice(device);
        return -1;
    }
    
    if (!RoboPeakUsbDisplayDriverIsSuccess(RoboPeakUsbDisplayEnable(device))) {
        RoboPeakUsbDisplayDisposeDevice(device);
        return -1;
    }
    
    while (true) {
        bool alive = false;
        
        if (!RoboPeakUsbDisplayDriverIsSuccess(RoboPeakUsbDisplayIsAlive(device, &alive))) {
            break;
        }
        
        if (!alive) {
            break;
        }
        
        if (!RoboPeakUsbDisplayDriverIsSuccess(RoboPeakUsbDisplayBitblt(device, 0, 0, 320, 240, RoboPeakUsbDisplayBitOperationCopy, framebuffer))) {
            break;
        }
        
        sleep(2);
        
        for (int i = 0; i < 100; i++) {
            uint16_t x = rand()%320;
            uint16_t y = rand()%240;
            uint16_t width = 1+(rand()%320);
            uint16_t height = 1+(rand()%240);
            uint16_t color = rand()&0xffffu;
            RoboPeakUsbDisplayBitOperation bitOperation = (RoboPeakUsbDisplayBitOperation)(rand()%4);
            
            if (!RoboPeakUsbDisplayDriverIsSuccess(RoboPeakUsbDisplayFillRect(device, x, y, x + width, y + height, color, bitOperation))) {
                break;
            }
        }
        
        sleep(2);
        
        if (!RoboPeakUsbDisplayDriverIsSuccess(RoboPeakUsbDisplayCopyArea(device, 0, 0, 160, 120, 160, 120))) {
            break;
        }
        
        sleep(2);
        
        if (!RoboPeakUsbDisplayDriverIsSuccess(RoboPeakUsbDisplayFill(device, 0xcb20u))) {
            break;
        }
        
        sleep(2);
    }
    
    RoboPeakUsbDisplayDisposeDevice(device);
    fprintf(stderr, "Display is disconnected\n");

    return 0;
}
#endif

int main(void) {
    uint16_t* framebuffer = (uint16_t*)malloc(320*240*2);
    uint16_t* p = framebuffer;
    
    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 320; x++, p++) {
            if (x == 8 || x == 311 || y == 8 || y == 231) {
                *p = 0xffff;
            } else {
                *p = 0xcb20u;
            }
        }
    }

#ifndef USE_C_INTERFACE
    int result = cPlusPlusInterfaceDemo(framebuffer);
#else
    int result = cInterfaceDemo(framebuffer);
#endif

    free(framebuffer);
    
    return result;
}
