//
//  device_handle.h
//  Device handle
//
//  Created by Tony Huang on 12/9/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <string>
#include <memory>
#include <rp/util/noncopyable.h>
#include <rp/deps/libusbx_wrap/enums.h>

extern "C" {
    struct libusb_device_handle;
}

namespace rp { namespace deps { namespace libusbx_wrap {

    class DeviceHandleImpl;
    class Device;
    class Transfer;
    
    class DeviceHandle : public std::enable_shared_from_this<DeviceHandle>, public rp::util::noncopyable {
    public:
        DeviceHandle(std::shared_ptr<Device> device, libusb_device_handle* deviceHandle);
        ~DeviceHandle();
        
        std::string getName();
        std::string getSerialNumber();
        
        void claimInterface(int usbInterface);
		void releaseInterface(int usbInterface);
        
        void clearEndpointHalt(uint8_t endpoint);
        
        std::shared_ptr<Device> getDevice();
        
        std::shared_ptr<Transfer> allocTransfer(EndpointDirection dir, EndpointTransferType type, uint8_t endpoint);
        
    private:
        std::unique_ptr<DeviceHandleImpl> impl_;
    };
    
}}}
