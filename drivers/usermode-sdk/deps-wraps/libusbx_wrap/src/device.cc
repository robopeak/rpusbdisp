//
//  device.cc
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/7/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <libusb.h>
#include <rp/deps/libusbx_wrap/device.h>
#include <rp/deps/libusbx_wrap/device_handle.h>
#include <rp/util/exception.h>

using namespace std;
using namespace rp::util;

namespace rp { namespace deps { namespace libusbx_wrap {
   
    class DeviceImpl : public rp::util::noncopyable {
    public:
        DeviceImpl(libusb_device* device) : device_(device) {
            libusb_ref_device(device);
        }
        
        ~DeviceImpl() {
            libusb_unref_device(device_);
            device_ = nullptr;
        }
        
        libusb_device_handle* open() {
            libusb_device_handle* handle;
            
            int result = libusb_open(device_, &handle);
            
            if (result < 0) {
                throw Exception(result);
            }
            
            return handle;
        }
        
        uint16_t getVid() {
            return getDescriptor().idVendor;
        }
        
        uint16_t getPid() {
            return getDescriptor().idProduct;
        }
        
        uint8_t getNameIndex() {
            return getDescriptor().iProduct;
        }
        
        uint8_t getSerialNumberIndex() {
            return getDescriptor().iSerialNumber;
        }
        
        uint16_t getFirmwareVersion() {
            return getDescriptor().bcdDevice;
        }
        
        int getMaxPacketSize(uint8_t endpoint) {
            return libusb_get_max_packet_size(device_, endpoint);
        }
        
    private:
        libusb_device_descriptor getDescriptor() {
            libusb_device_descriptor descriptor;
            
            int result = libusb_get_device_descriptor(device_, &descriptor);
            
            if (result) {
                throw Exception(result);
            }

            return descriptor;
        }
        
        libusb_device* device_;
    };
    
    Device::Device(libusb_device* device) : impl_(new DeviceImpl(device)) {}
    Device::~Device() {}
    
    uint16_t Device::getVid() {
        return impl_->getVid();
    }
    
    uint16_t Device::getPid() {
        return impl_->getPid();
    }
    
    uint8_t Device::getNameIndex() {
        return impl_->getNameIndex();
    }
    
    uint8_t Device::getSerialNumberIndex() {
        return impl_->getSerialNumberIndex();
    }
    
    int Device::getMaxPacketSize(uint8_t endpoint) {
        return impl_->getMaxPacketSize(endpoint);
    }
    
    uint16_t Device::getFirmwareVersion() {
        return impl_->getFirmwareVersion();
    }
    
    shared_ptr<DeviceHandle> Device::open() {
        libusb_device_handle* handler = impl_->open();
        
        return shared_ptr<DeviceHandle>(new DeviceHandle(shared_from_this(), handler));
    }
    
}}}
