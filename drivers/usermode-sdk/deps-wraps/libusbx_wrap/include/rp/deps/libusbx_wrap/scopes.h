//
//  scopes.h
//  Scopes
//
//  Created by Tony Huang on 12/10/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <memory>
#include <rp/util/noncopyable.h>

namespace rp { namespace deps { namespace libusbx_wrap {
   
    class DeviceHandle;

    class InterfaceScopeImpl;
    
    class InterfaceScope : public rp::util::noncopyable {
    public:
        InterfaceScope(std::shared_ptr<DeviceHandle> device, int usbInterface);
        ~InterfaceScope();
        
    private:
        std::unique_ptr<InterfaceScopeImpl> impl_;
    };
    
}}}
