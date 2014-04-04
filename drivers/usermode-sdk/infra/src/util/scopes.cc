//
//  scopes.cc
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/16/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <rp/util/scopes.h>
#include <rp/util/buffer.h>

using namespace std;

namespace rp { namespace util {

    class BufferLockScopeImpl : public noncopyable {
    public:
        BufferLockScopeImpl(shared_ptr<Buffer> buffer) : buffer_(buffer) {
            nakedBuffer_ = buffer->lock();
        }
        
        ~BufferLockScopeImpl() {
            buffer_->unlock(nakedBuffer_);
        }
        
        void* getBuffer() {
            return nakedBuffer_;
        }
        
    private:
        shared_ptr<Buffer> buffer_;
        void* nakedBuffer_;
    };
    
    BufferLockScope::BufferLockScope(shared_ptr<Buffer> buffer) : impl_(new BufferLockScopeImpl(buffer)) {}
    BufferLockScope::~BufferLockScope() {}
    
    void* BufferLockScope::getBuffer() {
        return impl_->getBuffer();
    }
    
}}
