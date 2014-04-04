//
//  buffer.cc
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/16/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <mutex>
#include <stdlib.h>
#include <string.h>
#include <rp/util/buffer.h>
#include <rp/util/exception.h>

using namespace std;

namespace rp { namespace util {
    
    class BufferImpl {
    public:
        BufferImpl(size_t size) : size_(size) {
            nakedBuffer_ = malloc(size);
            
            if (!nakedBuffer_) {
                throw Exception(-1, "Failed to alloc buffer");
            }
        }
        
        BufferImpl(const BufferImpl& that) : size_(that.size_) {
            nakedBuffer_ = malloc(size_);
            
            if (!nakedBuffer_) {
                throw Exception(-1, "Failed to alloc buffer");
            }
            
            memcpy(nakedBuffer_, that.nakedBuffer_, size_);
        }
        
        ~BufferImpl() {
            lock_guard<mutex> guard(lock_);
            
            if (nakedBuffer_) {
                free(nakedBuffer_);
                nakedBuffer_ = 0;
            }
        }
        
        void* lock() {
            lock_.lock();
            return nakedBuffer_;
        }
        
        void unlock(void* nakedBuffer) {
            if (nakedBuffer_ != nakedBuffer) {
                throw Exception(-1, "Unlocking wrong naked buffer");
            }
            lock_.unlock();
        }
        
        size_t size() const {
            return size_;
        }
        
    private:
        mutex lock_;
        void* nakedBuffer_;
        size_t size_;
    };
    
    Buffer::Buffer(size_t size) : impl_(new BufferImpl(size)) {}
    Buffer::Buffer(const Buffer& that) : impl_(new BufferImpl(*that.impl_)) {}
    Buffer::~Buffer() {}
    
    Buffer& Buffer::operator=(const Buffer &that) {
        impl_ = unique_ptr<BufferImpl>(new BufferImpl(*that.impl_));
        return *this;
    }
    
    void* Buffer::lock() {
        return impl_->lock();
    }
    
    void Buffer::unlock(void* nakedBuffer) {
        impl_->unlock(nakedBuffer);
    }
    
    size_t Buffer::size() const {
        return impl_->size();
    }
    
}}

