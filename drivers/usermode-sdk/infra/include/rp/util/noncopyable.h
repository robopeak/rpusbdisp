//
//  noncopyable.h
//  noncopyable
//
//  Created by Tony Huang on 12/9/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <rp/infra_config.h>

namespace rp { namespace util {

    /**
     * \brief A base class to avoid noncopyable classes to be copied
     *
     * A lot of classes we wrote shouldn't be copied (such as File which wraps a native file descriptor).
     * by inherting from this class, the default copy constructor and the operator= are deleted to avoid such situation
     */
	class RP_INFRA_API noncopyable {
    public:
        noncopyable() {}
        ~noncopyable() {}
        
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };
    
}}
