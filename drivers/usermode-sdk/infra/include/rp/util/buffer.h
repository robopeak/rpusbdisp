//
//  buffer.h
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/16/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <rp/infra_config.h>
#include <memory>

namespace rp { namespace util {

    class BufferImpl;
    
    /**
     * \brief Memory buffer
     *
     * Buffer is very similar to the native C array.
     * The size of Buffer is fixed when the Buffer is created, and you can use copy contructor or assign operator to create a clone of a Buffer
     * The only way to access the internal data of Buffer is to use the lock() method to lock the data and avoid other thread's access to the data.
     * After usage, you need to invoke the unlock method to release the buffer.
     */
	class RP_INFRA_API Buffer {
    public:
        /**
         * \brief Create a buffer
         *
         * Create a buffer of particular size. The memory area is just allocated and uninitialized, so please don't assume it's zero filled or 0xff filled.
         *
         * \param size The size of the buffer
         */
        Buffer(size_t size);
        
        /**
         * \brief Create a buffer by cloning another buffer
         *
         * Create a buffer with the same size, and copy all data from another buffer to make a clone.
         *
         * \param that The other buffer
         */
        Buffer(const Buffer& that);
        
        /**
         * Release the buffer
         */
        ~Buffer();
        
        /**
         * Clone another buffer's data and replace current buffer
         *
         * \param that The other buffer
         */
        Buffer& operator=(const Buffer& that);
        
        /**
         * Lock the buffer and get the pointer to the inner buffer
         */
        void* lock();
        
        /**
         * \brief Unlock the locked buffer
         *
         * Unlock the locked buffer. Please note that the buffer param should exactly match the return value of lock() method when you lock the buffer.
         */
        void unlock(void* buffer);
        
        /**
         * The size of the buffer (in bytes)
         */
        size_t size() const;
        
    private:
        std::unique_ptr<BufferImpl> impl_;
    };
    
}}
