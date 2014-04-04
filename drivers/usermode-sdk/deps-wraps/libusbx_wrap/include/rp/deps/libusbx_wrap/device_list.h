//
//  device_list.h
//  Device list
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

    class DeviceListImpl;
    class Device;
    
    class DeviceList : public rp::util::noncopyable {
    public:
        DeviceList(size_t, libusb_device**);
        ~DeviceList();
        
        std::shared_ptr<Device> getDevice(size_t index);
        size_t count() const;
        
    private:
        std::unique_ptr<DeviceListImpl> impl_;
    };
    
}}}
