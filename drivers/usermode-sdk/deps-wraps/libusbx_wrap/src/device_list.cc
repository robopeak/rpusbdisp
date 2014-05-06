//
//  device_list.cc
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/7/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <libusb.h>

#include <rp/deps/libusbx_wrap/device_list.h>
#include <rp/deps/libusbx_wrap/device.h>

using namespace std;

namespace rp { namespace deps { namespace libusbx_wrap {
    
    class DeviceListImpl : public rp::util::noncopyable {
    public:
        DeviceListImpl(size_t count, libusb_device** list) : size_(count), list_(list) {}
        ~DeviceListImpl() {
            libusb_free_device_list(list_, 1);
            list_ = 0;
            size_ = 0;
        }
        
        shared_ptr<Device> getDevice(size_t index) {
            if (index >= size_) {
                return shared_ptr<Device>(nullptr);
            }
            
            return shared_ptr<Device>(new Device(list_[index]));
        }
        
        size_t count() const {
            return size_;
        }
        
    private:
        size_t size_;
        libusb_device** list_;
        
    };
    
    DeviceList::DeviceList(size_t count, libusb_device** list) : impl_(new DeviceListImpl(count, list)) {}
    DeviceList::~DeviceList() {}
    
    shared_ptr<Device> DeviceList::getDevice(size_t index) {
        return impl_->getDevice(index);
    }
    
    size_t DeviceList::count() const {
        return impl_->count();
    }
    
    
}}}

