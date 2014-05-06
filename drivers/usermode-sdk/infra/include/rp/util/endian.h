//
//  endian.h
//  Deal with endian issues
//
//  Created by Tony Huang on 12/11/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <rp/infra_config.h>
#include <rp/util/int_types.h>

namespace rp { namespace util {

    /**
     * Convert unsigned 16bit integer from little endian to cpu endian
     */
    static inline uint16_t le16_to_cpu(uint16_t v) {
#ifdef RP_INFRA_BIG_ENDIAN
        return (v>>8) | ((v&0xffu)<<8);
#else
        return v;
#endif
    }
    
    /**
     * Convert unsigned 16bit integer from cpu endian to little endian
     */
    static inline uint16_t cpu_to_le16(uint16_t v) {
        return le16_to_cpu(v);
    }
    
    /**
     * Convert unsigned 32bit integer from little endian to cpu endian
     */
    static inline uint32_t le32_to_cpu(uint32_t v) {
#ifdef RP_INFRA_BIG_ENDIAN
        return (v>>24) | (((v>>16)&0xffu)<<8) | (((v>>8)&0xffu)<<16) | ((v&0xffu)<<24);
#else
        return v;
#endif
    }
    
    /**
     * Convert unsigned 32bit integer from cpu endian to little endian
     */
    static inline uint32_t cpu_to_le32(uint32_t v) {
        return le32_to_cpu(v);
    }
    
}}

