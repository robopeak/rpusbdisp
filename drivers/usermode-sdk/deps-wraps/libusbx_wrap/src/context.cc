//
//  context.cc
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/7/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <mutex>
#include <atomic>
#include <thread>
#include <functional>
#include <libusb.h>
#include <rp/deps/libusbx_wrap/context.h>
#include <rp/deps/libusbx_wrap/device_list.h>
#include <rp/deps/libusbx_wrap/device.h>
#include <rp/util/exception.h>

using namespace std;
using namespace rp::util;

namespace rp { namespace deps { namespace libusbx_wrap {
    
    class ContextImpl : public noncopyable {
    public:
        ContextImpl() {
            ctx_ = 0;
            int result = libusb_init(&ctx_);
            if (result) {
                throw Exception(result);
            }
        }
        ~ContextImpl() {
            libusb_exit(ctx_);
            ctx_ = 0;
        };
        
        shared_ptr<DeviceList> getDeviceList() {
            libusb_device** devices;
            ssize_t count = libusb_get_device_list(ctx_, &devices);
            
            if (count < 0) {
                throw Exception((int)count);
            }
            
            return shared_ptr<DeviceList>(new DeviceList((size_t)count, devices));
        }
        
        vector<shared_ptr<Device> > lookupDevices(uint16_t vid, uint16_t pid) {
            shared_ptr<DeviceList> devices = this->getDeviceList();
            
            vector<shared_ptr<Device>> output;
            
            for (size_t i = 0; i < devices->count(); i++) {
                shared_ptr<Device> device = devices->getDevice(i);
                if (device->getVid() == vid && device->getPid() == pid) {
                    output.push_back(device);
                }
            }
            
            return output;
        }
        
        shared_ptr<Pipeline> summonPipeline(shared_ptr<Context> context) {
            lock_guard<mutex> guard(pipelineMutex_);
            
            shared_ptr<Pipeline> pipeline = pipeline_.lock();
            
            if (!pipeline) {
                shared_ptr<Pipeline> pipeline(new Pipeline(context));
                pipeline_ = pipeline;
                return pipeline;
            } else {
                return pipeline;
            }
        }
        
        void poll() {
			timeval timeout;
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

			if (libusb_handle_events_timeout(ctx_, &timeout)) {
                std::this_thread::sleep_for(chrono::microseconds(100));
            }
        }
        
    private:
        mutex pipelineMutex_;
        weak_ptr<Pipeline> pipeline_;
        libusb_context* ctx_;
    };
    
    static shared_ptr<Context> context_;
    static once_flag onceFlag_;
    
    Context::Context() : impl_(new ContextImpl()) {}
    Context::~Context() {}
    
    shared_ptr<DeviceList> Context::getDeviceList() {
        return impl_->getDeviceList();
    }
    
    vector<shared_ptr<Device>> Context::lookupDevices(uint16_t vid, uint16_t pid) {
        return impl_->lookupDevices(vid, pid);
    }
    
    shared_ptr<Pipeline> Context::summonPipeline() {
        return impl_->summonPipeline(shared_from_this());
    }
    
    void Context::poll() {
        impl_->poll();
    }
    
    void Context::createDefaultContext_() {
        context_ = shared_ptr<Context>(new Context());
    }
    
    shared_ptr<Context> Context::defaultContext() {
        call_once(onceFlag_, &Context::createDefaultContext_);
        return context_;
    }
    
    
    class PipelineImpl : public noncopyable {
    public:
        PipelineImpl(shared_ptr<Context> context) : context_(context), working_(false) {}
        ~PipelineImpl() {
            if (thread_.joinable()) {
                thread_.join();
            }
        }
        
        void start() {
            working_.store(true);
            
            call_once(onceFlag_, bind(&PipelineImpl::startThread_, this));
        }
        
        void stop() {
            working_.store(false);
        }
        
        void workThread() {
            while (working_.load()) {
                context_->poll();
            }
        }
        
    private:
        void startThread_() {
            this->thread_ = move(thread(bind(&PipelineImpl::workThread, this)));
        }
        
        once_flag onceFlag_;
        thread thread_;
        atomic<bool> working_;
        shared_ptr<Context> context_;
    };
    
    Pipeline::Pipeline(shared_ptr<Context> context) : impl_(new PipelineImpl(context)) {}
    Pipeline::~Pipeline() {
        impl_->stop();
    }
    
    void Pipeline::start() {
        impl_->start();
    }
    
}}}