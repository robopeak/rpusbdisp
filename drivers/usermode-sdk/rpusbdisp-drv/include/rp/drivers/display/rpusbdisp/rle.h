//
//  rle.h
//  RLE compression algorithm used by rpusbdisp
//
//  Created by Tony Huang on 12/11/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <memory>

namespace rp { namespace util {

    class Buffer;
    
}}

namespace rp { namespace drivers { namespace display {
    
    /**
     * Compress data with RLE algorithm and return the compressed data
     */
    std::shared_ptr<rp::util::Buffer> rleCompress(std::shared_ptr<rp::util::Buffer> buffer);
    
}}}
