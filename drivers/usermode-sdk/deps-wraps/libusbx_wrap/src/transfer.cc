//
//  transfer.cc
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/10/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <condition_variable>
#include <functional>
#include <libusb.h>
#include <rp/util/buffer.h>
#include <rp/util/scopes.h>
#include <rp/deps/libusbx_wrap/transfer.h>
#include <rp/util/exception.h>

using namespace std;
using namespace rp::util;

namespace rp { namespace deps { namespace libusbx_wrap {

    class TransferImpl : public enable_shared_from_this<TransferImpl>, public noncopyable {
    public:
        TransferImpl(shared_ptr<DeviceHandle> deviceHandle, libusb_transfer* transfer) : completion_(false), deviceHandle_(deviceHandle), handle_(transfer) {
            handle_->timeout = 0;
            handle_->user_data = 0;
            handle_->callback = &TransferImpl::transferCallback_;
        }
        
        ~TransferImpl() {
            releaseTransferBuffer_();
            if (handle_ && handle_->user_data) {
                delete reinterpret_cast<weak_ptr<TransferImpl>*>(handle_->user_data);
                handle_->user_data = nullptr;
            }
            libusb_free_transfer(handle_);
        }
    
        void setTransferBuffer(shared_ptr<Buffer> transferBuffer) {
            releaseTransferBuffer_();
            
            transferBuffer_ = transferBuffer;
            if (transferBuffer) {
                handle_->buffer = (uint8_t*)transferBuffer->lock();
            }
            handle_->length = (int)transferBuffer->size();
        }
        
        shared_ptr<Buffer> getTransferBuffer() {
            return transferBuffer_;
        }
        
        void submit() {
            if (!handle_->user_data) {
                handle_->user_data = new weak_ptr<TransferImpl>(shared_from_this());
            }
            
            int result = libusb_submit_transfer(handle_);
            
            if (result) {
                throw Exception(result);
            }
        }
        
        void waitForCompletion() {
            unique_lock<mutex> lock(conditionLock_);
            condition_.wait(lock, bind(&TransferImpl::isCompleted_, this));
            this->completion_ = false;
        }
        
        TransferStatus getStatus() {
            switch (handle_->status) {
                case LIBUSB_TRANSFER_COMPLETED:
                    return TransferStatusCompleted;
                case LIBUSB_TRANSFER_CANCELLED:
                    return TransferStatusCancelled;
                case LIBUSB_TRANSFER_ERROR:
                    return TransferStatusError;
                case LIBUSB_TRANSFER_TIMED_OUT:
                    return TransferStatusTimeout;
                case LIBUSB_TRANSFER_STALL:
                    return TransferStatusStall;
                case LIBUSB_TRANSFER_NO_DEVICE:
                    return TransferStatusDeviceError;
                case LIBUSB_TRANSFER_OVERFLOW:
                    return TransferStatusOverflow;
                default:
                    return TransferStatusUnknown;
            }
        }
        
    private:
        void releaseTransferBuffer_() {
            if (transferBuffer_.get()) {
                transferBuffer_->unlock(handle_->buffer);
                handle_->buffer = 0;
                handle_->length = 0;
                transferBuffer_ = nullptr;
            }
        }
        
        void triggerCompletion_() {
            {
                lock_guard<mutex> guard(conditionLock_);
                completion_ = true;
            }
            condition_.notify_one();
        }
        
        bool isCompleted_() {
            return completion_;
        }
        
		static void RP_INFRA_CALLBACK transferCallback_(libusb_transfer* data) {
            weak_ptr<TransferImpl>* weakPtr = reinterpret_cast<weak_ptr<TransferImpl>*>(data->user_data);
            if (!weakPtr) return;
            
            shared_ptr<TransferImpl> transfer = weakPtr->lock();
            
            if (!transfer) return;
            
            transfer->triggerCompletion_();
        }
        
        mutex conditionLock_;
        condition_variable condition_;
        bool completion_;
        
        shared_ptr<DeviceHandle> deviceHandle_;
        shared_ptr<Buffer> transferBuffer_;
        libusb_transfer* handle_;
    };
    
    Transfer::Transfer(shared_ptr<DeviceHandle> deviceHandle, libusb_transfer* transfer) : impl_(new TransferImpl(deviceHandle, transfer)) {}
    Transfer::~Transfer() {}
    
    void Transfer::setTransferBuffer(shared_ptr<Buffer> buffer) {
        impl_->setTransferBuffer(buffer);
    }
    
    shared_ptr<Buffer> Transfer::getTransferBuffer() {
        return impl_->getTransferBuffer();
    }
    
    void Transfer::submit() {
        impl_->submit();
    }
    
    void Transfer::waitForCompletion() {
        impl_->waitForCompletion();
    }
    
    TransferStatus Transfer::getStatus() {
        return impl_->getStatus();
    }
    
}}}
