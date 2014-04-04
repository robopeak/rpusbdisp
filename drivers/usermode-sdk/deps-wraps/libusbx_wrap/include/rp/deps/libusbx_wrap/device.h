//
//  device.h
//  A usb device in libusb
//
//  Created by Tony Huang on 12/7/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <rp/util/noncopyable.h>
#include <memory>

extern "C" {
    struct libusb_device;
}

namespace rp { namespace deps { namespace libusbx_wrap {
    
    class DeviceHandle;
    class DeviceImpl;
    
    class Device : public std::enable_shared_from_this<Device>, public rp::util::noncopyable {
    public:
        Device(libusb_device*);
        ~Device();
        
        std::shared_ptr<DeviceHandle> open();
        
        uint16_t getVid();
        uint16_t getPid();
        uint8_t getNameIndex();
        uint8_t getSerialNumberIndex();
        int getMaxPacketSize(uint8_t endpoint);
        uint16_t getFirmwareVersion();
        
    private:
        std::unique_ptr<DeviceImpl> impl_;
    };
    
}}}