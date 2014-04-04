//
//  scopes.h
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/16/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <rp/infra_config.h>
#include <memory>
#include <rp/util/noncopyable.h>

namespace rp { namespace util {
    
    class Buffer;
    class BufferLockScopeImpl;
    
    /**
     * \brief The RAII way to lock Buffers
     *
     * BufferLockScope is a RAII way to access the inner buffer of Buffers. It will automatically lock and unlock buffer by the automatic lifetime management of BufferLockScope in C++.
     *
     * Example
     * \code{.cpp}
     * void fillZero(shared_ptr<Buffer> buffer) {
     *     BufferLockScope scope(buffer);
     *     memset(scope.getBuffer(), 0, buffer->size());
     * }
     * \endcode
     */
	class RP_INFRA_API BufferLockScope : public rp::util::noncopyable {
    public:
        /**
         * Construct a lock scope
         *
         * \param buffer The Buffer to be locked
         */
        BufferLockScope(std::shared_ptr<Buffer> buffer);
        
        /**
         * Automatically unlock Buffer;
         */
        ~BufferLockScope();
        
        /**
         * Get the pointer to the inner buffer of the locked Buffer
         */
        void* getBuffer();
        
    private:
        std::unique_ptr<BufferLockScopeImpl> impl_;
    };
    
}}
