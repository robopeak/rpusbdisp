//
//  scopes.cc
//  Scopes
//
//  Created by Tony Huang on 12/10/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <rp/deps/libusbx_wrap/scopes.h>
#include <rp/deps/libusbx_wrap/device_handle.h>

using namespace std;
using namespace rp::util;

namespace rp { namespace deps { namespace libusbx_wrap {
    
    class InterfaceScopeImpl : noncopyable {
    public:
		InterfaceScopeImpl(shared_ptr<DeviceHandle> device, int usbInterface) : deviceHandle_(device), interface_(usbInterface) {
			device->claimInterface(usbInterface);
        }
        
        ~InterfaceScopeImpl() {
            deviceHandle_->releaseInterface(interface_);
        }
        
    private:
        std::shared_ptr<DeviceHandle> deviceHandle_;
        int interface_;
    };
    
	InterfaceScope::InterfaceScope(shared_ptr<DeviceHandle> device, int usbInterface) : impl_(new InterfaceScopeImpl(device, usbInterface)) {}
    
    InterfaceScope::~InterfaceScope() {}
    
}}}
