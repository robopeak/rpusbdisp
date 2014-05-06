//
//  transfer.h
//  Wrap of libusb_transfer* apis
//
//  Created by Tony Huang on 12/10/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <memory>
#include <rp/util/noncopyable.h>
#include <rp/deps/libusbx_wrap/enums.h>

extern "C" {
    struct libusb_transfer;
}

namespace rp { namespace util {
    
    class Buffer;
    
}}

namespace rp { namespace deps { namespace libusbx_wrap {
   
    class DeviceHandle;
    class TransferImpl;
    
    class Transfer : public rp::util::noncopyable {
    public:
        Transfer(std::shared_ptr<DeviceHandle> deviceHandle, libusb_transfer* transfer);
        ~Transfer();
        
        void setTransferBuffer(std::shared_ptr<rp::util::Buffer> buffer);
        std::shared_ptr<rp::util::Buffer> getTransferBuffer();
        
        void submit();
        void waitForCompletion();
        TransferStatus getStatus();
        
    private:
        std::shared_ptr<TransferImpl> impl_;
    };
    
}}}
