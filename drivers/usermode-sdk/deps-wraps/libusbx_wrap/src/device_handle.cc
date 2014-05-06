//
//  device_handle.cc
//  Device Handle
//
//  Created by Tony Huang on 12/9/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <rp/deps/libusbx_wrap/device_handle.h>
#include <rp/deps/libusbx_wrap/device.h>
#include <rp/deps/libusbx_wrap/transfer.h>
#include <rp/util/exception.h>
#include <libusb.h>

using namespace std;
using namespace rp::util;

namespace rp { namespace deps { namespace libusbx_wrap {

    class DeviceHandleImpl : public rp::util::noncopyable {
    public:
        DeviceHandleImpl(shared_ptr<Device> device, libusb_device_handle* deviceHandle) : device_(device), handle_(deviceHandle) {}
        ~DeviceHandleImpl() {
            libusb_close(handle_);
        }
        
        string getName() {
            return getStringDescriptorAscii(device_->getNameIndex());
        }
        
        string getSerialNumber() {
            return getStringDescriptorAscii(device_->getSerialNumberIndex());
        }
        
        void claimInterface(int interface) {
            int result = libusb_claim_interface(handle_, interface);
            
            if (result) {
                throw Exception(result);
            }
        }
        
        void releaseInterface(int interface) {
            int result = libusb_release_interface(handle_, interface);
            
            if (result) {
                throw Exception(result);
            }
        }
        
        void clearEndpointHalt(uint8_t endpoint) {
            int result = libusb_clear_halt(handle_, endpoint);
            
            if (result) {
                throw Exception(result);
            }
        }
        
        shared_ptr<Device> getDevice() {
            return device_;
        }
        
        libusb_transfer* allocTransfer(EndpointDirection dir, EndpointTransferType type, uint8_t endpoint) {
            libusb_transfer* transfer = libusb_alloc_transfer(0);
            
            if (!transfer) {
                throw Exception(-1);
            }
            
            transfer->dev_handle = handle_;
            
            if (dir == EndpointDirectionIn) {
                endpoint |= 0x80u;
            } else {
                endpoint &= 0x0fu;
            }
            
            transfer->endpoint = endpoint;
            transfer->type = (uint8_t)type;
            
            return transfer;
        }
        
    private:
        string getStringDescriptorAscii(uint8_t index) {
            char stringDescriptor[256];
            int read = libusb_get_string_descriptor_ascii(handle_, index, (unsigned char*)stringDescriptor, 256);
            
            if (read < 0) {
                throw Exception(read);
            } else {
                stringDescriptor[read] = 0;
            }
            
            return stringDescriptor;
        }
        
        shared_ptr<Device> device_;
        libusb_device_handle* handle_;
    };
    
    DeviceHandle::DeviceHandle(shared_ptr<Device> device, libusb_device_handle* deviceHandle)
    : impl_(new DeviceHandleImpl(device, deviceHandle))
    {}
    
    DeviceHandle::~DeviceHandle() { }
    
    string DeviceHandle::getName() {
        return impl_->getName();
    }
    
    string DeviceHandle::getSerialNumber() {
        return impl_->getSerialNumber();
    }
    
    void DeviceHandle::claimInterface(int interface) {
        impl_->claimInterface(interface);
    }
    
    void DeviceHandle::releaseInterface(int interface) {
        impl_->releaseInterface(interface);
    }
    
    void DeviceHandle::clearEndpointHalt(uint8_t endpoint) {
        impl_->clearEndpointHalt(endpoint);
    }
    
    shared_ptr<Device> DeviceHandle::getDevice() {
        return impl_->getDevice();
    }
    
    shared_ptr<Transfer> DeviceHandle::allocTransfer(EndpointDirection dir, EndpointTransferType type, uint8_t endpoint) {
        auto nakedTransfer = impl_->allocTransfer(dir, type, endpoint);
        
        return shared_ptr<Transfer>(new Transfer(shared_from_this(), nakedTransfer));
    }
    
}}}
