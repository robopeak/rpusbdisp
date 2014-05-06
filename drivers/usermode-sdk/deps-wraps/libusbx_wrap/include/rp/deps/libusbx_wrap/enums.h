//
//  enums.h
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/10/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

namespace rp { namespace deps { namespace libusbx_wrap {
    
    enum EndpointDirection {
        EndpointDirectionIn = 0x80,
        EndpointDirectionOut = 0x00
    };
    
    enum EndpointTransferType {
        EndpointTransferTypeControl = 0,
        EndpointTransferTypeIsochronous = 1,
        EndpointTransferTypeBulk = 2,
        EndpointTransferTypeInterrupt = 3
    };
    
    enum TransferStatus {
        TransferStatusUnknown = 0,
        TransferStatusCompleted = 1,
        TransferStatusCancelled = 2,
        TransferStatusError = 3,
        TransferStatusTimeout = 4,
        TransferStatusStall = 5,
        TransferStatusDeviceError = 6,
        TransferStatusOverflow = 7
    };

}}}
