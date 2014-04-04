//
//  context.h
//  A C++ wrap for libusbx context
//
//  Created by Tony Huang on 12/7/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <rp/util/noncopyable.h>
#include <memory>
#include <vector>

namespace rp { namespace deps { namespace libusbx_wrap {
  
    class ContextImpl;
    class DeviceList;
    class Device;
    class Pipeline;
    class PipelineImpl;
    
    class Context : public std::enable_shared_from_this<Context>, public rp::util::noncopyable {
    private:
        Context();

    public:
        ~Context();
        
        std::shared_ptr<DeviceList> getDeviceList();
        std::vector<std::shared_ptr<Device> > lookupDevices(uint16_t vid, uint16_t pid);
        
        std::shared_ptr<Pipeline> summonPipeline();
        
        void poll();

        static std::shared_ptr<Context> defaultContext();
        
    private:
        static void createDefaultContext_();
        std::unique_ptr<ContextImpl> impl_;
    };
    
    class Pipeline : public std::enable_shared_from_this<Pipeline>, public rp::util::noncopyable {
    public:
        Pipeline(std::shared_ptr<Context> context);
        ~Pipeline();
        
        void start();
        
    private:
        std::unique_ptr<PipelineImpl> impl_;
    };
    
}}}