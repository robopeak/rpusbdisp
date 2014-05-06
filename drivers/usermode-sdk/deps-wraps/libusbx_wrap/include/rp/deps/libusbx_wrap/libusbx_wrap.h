//
//  libusbx_wrap.h
//  Overall header file to make it easier to use
//
//  Created by Tony Huang on 12/11/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <rp/deps/libusbx_wrap/context.h>
#include <rp/deps/libusbx_wrap/device_list.h>
#include <rp/deps/libusbx_wrap/device.h>
#include <rp/deps/libusbx_wrap/device_handle.h>
#include <rp/deps/libusbx_wrap/scopes.h>
#include <rp/deps/libusbx_wrap/transfer.h>
#include <rp/util/buffer.h>
#include <rp/util/scopes.h>
#include <rp/util/exception.h>

namespace rp { namespace deps { namespace libusbx_wrap {
   
    template<typename T>
    static T decodeAs(std::shared_ptr<Transfer> transfer) {
        std::shared_ptr<rp::util::Buffer> transferBuffer(new rp::util::Buffer(*transfer->getTransferBuffer()));
        rp::util::BufferLockScope lockScope(transferBuffer);
        
        T output;
        memcpy(&output, lockScope.getBuffer(), sizeof(T));
        
        return output;
    }
    
}}}