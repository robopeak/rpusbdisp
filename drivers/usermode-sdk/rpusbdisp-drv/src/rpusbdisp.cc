//
//  rpusbdisp.cc
//  RoboPeak Mini USB Display Driver
//
//  Created by Tony Huang on 12/11/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <rp/drivers/display/rpusbdisp/rpusbdisp.h>
#include <rp/deps/libusbx_wrap/libusbx_wrap.h>
#include <rp/drivers/display/rpusbdisp/rle.h>
#include <rp/util/endian.h>
#include <rp/util/buffer.h>
#include <stdio.h>
#include <memory.h>

#define RP_USB_DISPLAY_VID    0xFCCFu
#define RP_USB_DISPLAY_PID    0xA001u

#define RP_USB_DISPLAY_STATUS_ENDPOINT  0x82u
#define RP_USB_DISPLAY_DISPLAY_ENDPOINT  0x01u

#define RP_USB_DISPLAY_WIDTH 320
#define RP_USB_DISPLAY_HEIGHT 240

#define RP_USB_DISPLAY_MIN_VERSION_FILL 0x0104u
#define RP_USB_DISPLAY_MIN_VERSION_BITBLT_RLE 0x0104u
#define RP_USB_DISPLAY_MIN_VERSION_COPY_AREA_BUG_FIX 0x0104u

using namespace std;
using namespace rp::util;
using namespace rp::deps::libusbx_wrap;

namespace rp { namespace drivers { namespace display {
    
    const uint16_t RoboPeakUsbDisplayDevice::UsbDeviceVendorId = RP_USB_DISPLAY_VID;
    const uint16_t RoboPeakUsbDisplayDevice::UsbDeviceProductId = RP_USB_DISPLAY_PID;
    
    const uint8_t RoboPeakUsbDisplayDevice::UsbDeviceStatusEndpoint = RP_USB_DISPLAY_STATUS_ENDPOINT;
    const uint8_t RoboPeakUsbDisplayDevice::UsbDeviceDisplayEndpoint = RP_USB_DISPLAY_DISPLAY_ENDPOINT;
    
    const int RoboPeakUsbDisplayDevice::ScreenWidth = RP_USB_DISPLAY_WIDTH;
    const int RoboPeakUsbDisplayDevice::ScreenHeight = RP_USB_DISPLAY_HEIGHT;

    class RoboPeakUsbDisplayDeviceImpl : public enable_shared_from_this<RoboPeakUsbDisplayDeviceImpl>, public noncopyable {
    public:
        RoboPeakUsbDisplayDeviceImpl(shared_ptr<DeviceHandle> device) : device_(device), interfaceScope_(device, 0) {
            status_.display_status = 0x80u;
            status_.touch_status = 0;
            status_.touch_x = 0;
            status_.touch_y = 0;
            
            working_.store(false);
            
            maxPacketSize_ = device->getDevice()->getMaxPacketSize(RoboPeakUsbDisplayDevice::UsbDeviceDisplayEndpoint);
        }
        ~RoboPeakUsbDisplayDeviceImpl() {
            working_.store(false);
            if (statusFetchingThread_.joinable()) {
                statusFetchingThread_.join();
            }
        }

        void setStatusUpdatedCallback(function<void(const rpusbdisp_status_normal_packet_t&)> callback) {
            lock_guard<mutex> guard(statusLock_);
            statusUpdatedCallback_ = callback;
        }
        
        rpusbdisp_status_normal_packet_t getStatus() {
            lock_guard<mutex> guard(statusLock_);
            return status_;
        }
        
        shared_ptr<Transfer> sendDataToDisplayEndpointNoWait(shared_ptr<Buffer> buffer) {
            shared_ptr<Transfer> transfer = device_->allocTransfer(EndpointDirectionOut, EndpointTransferTypeBulk, 0x01);
            transfer->setTransferBuffer(buffer);
            
            shared_ptr<Pipeline> pipeline = Context::defaultContext()->summonPipeline();
            pipeline->start();
            
            transfer->submit();
            transfer->waitForCompletion();

            return transfer;
        }
        
        void sendDataToDisplayEndpoint(shared_ptr<Buffer> buffer) {
            auto transfer = sendDataToDisplayEndpointNoWait(buffer);
            
            switch(transfer->getStatus()) {
                case deps::libusbx_wrap::TransferStatusCompleted:
                    break;
                default:
                    throw Exception(transfer->getStatus());
            }
        }
        
        template<typename PacketT>
        void sendCommandToDisplayEndpoint(PacketT& packet, shared_ptr<Buffer> payload=nullptr) {
            size_t totalSize = sizeof(PacketT);
            if (payload) {
                totalSize += payload->size();
            }
            size_t packetCount = (totalSize-1+maxPacketSize_-1)/(maxPacketSize_-1);
            
            shared_ptr<Buffer> overallBuffer(new Buffer(totalSize));
            
            BufferLockScope scope(overallBuffer);
            _u8* scopeNakedBuffer = (_u8*)scope.getBuffer();
            memcpy(scopeNakedBuffer, &packet, sizeof(PacketT));
            if (payload) {
                BufferLockScope payloadScope(payload);
                memcpy(scopeNakedBuffer+sizeof(PacketT), payloadScope.getBuffer(), payload->size());
            }
            
            size_t transferBufferSize = totalSize + packetCount - 1; // every extra packet cost one more byte to store command
            shared_ptr<Buffer> transferBuffer(new Buffer(transferBufferSize));
            
            {
                BufferLockScope transferBufferLockScope(transferBuffer);
                _u8* nakedTransferBuffer = (_u8*)transferBufferLockScope.getBuffer();
                
                for (size_t packetIndex = 0; packetIndex < packetCount; packetIndex++) {
                    size_t packetOffset = packetIndex * (maxPacketSize_ - 1) + 1;
                    size_t payloadSize = maxPacketSize_ - 1;
                    if (payloadSize > totalSize - packetOffset)
                        payloadSize = totalSize - packetOffset;
                    
                    size_t transferBufferOffset = packetIndex * maxPacketSize_;
                    nakedTransferBuffer[transferBufferOffset] = scopeNakedBuffer[0];
                    if (packetIndex == 0) {
                        nakedTransferBuffer[transferBufferOffset] |= RPUSBDISP_CMD_FLAG_START;
                        {
                            lock_guard<mutex> guard(statusLock_);
                            if (status_.display_status & RPUSBDISP_DISPLAY_STATUS_DIRTY_FLAG) {
                                nakedTransferBuffer[transferBufferOffset] |= RPUSBDISP_CMD_FLAG_CLEARDITY;
                            }
                        }
                    }
                    
                    memcpy(nakedTransferBuffer+transferBufferOffset+1, scopeNakedBuffer+packetOffset, payloadSize);
                }
            }
            
            shared_ptr<Transfer> transfer = device_->allocTransfer(EndpointDirectionOut, EndpointTransferTypeBulk, RoboPeakUsbDisplayDevice::UsbDeviceDisplayEndpoint);
            transfer->setTransferBuffer(transferBuffer);
            transfer->submit();
            
            transfer->waitForCompletion();

            switch (transfer->getStatus()) {
                case deps::libusbx_wrap::TransferStatusCompleted:
                    return;
                default:
                    throw Exception(transfer->getStatus());
            }
        }
        
        void fill(uint16_t color) {
            if (device_->getDevice()->getFirmwareVersion() < RP_USB_DISPLAY_MIN_VERSION_FILL) {
                int width = getWidth(), height = getHeight();
                fillrect(0, 0, width-1, height-1, color, RoboPeakUsbDisplayBitOperationCopy);
            } else {
                rpusbdisp_disp_fill_packet_t fillPacket;
                
                fillPacket.header.cmd_flag = RPUSBDISP_DISPCMD_FILL;
                fillPacket.color_565 = cpu_to_le16(color);
                
                sendCommandToDisplayEndpoint(fillPacket);
            }
        }
        
        void bitblt(uint16_t x, uint16_t y, uint16_t width, uint16_t height, RoboPeakUsbDisplayBitOperation bitOperation, void* buffer) {
            size_t payloadSize = (size_t)(width * height * 2);
            
            shared_ptr<Buffer> payload(new Buffer(payloadSize));
            
            {
                BufferLockScope scope(payload);
                memcpy(scope.getBuffer(), buffer, payloadSize);
            }
            
            rpusbdisp_disp_bitblt_packet_t packet;
            
            if (device_->getDevice()->getFirmwareVersion() < RP_USB_DISPLAY_MIN_VERSION_BITBLT_RLE) {
                packet.header.cmd_flag = RPUSBDISP_DISPCMD_BITBLT;
            } else {
                packet.header.cmd_flag = RPUSBDISP_DISPCMD_BITBLT_RLE;
                payload = rleCompress(payload);
            }
            
            packet.x = cpu_to_le16(x);
            packet.y = cpu_to_le16(y);
            packet.width = cpu_to_le16(width);
            packet.height = cpu_to_le16(height);
            packet.operation = (_u8)bitOperation;
            
            sendCommandToDisplayEndpoint(packet, payload);
        }
        
        void fillrect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t color, RoboPeakUsbDisplayBitOperation bitOperation) {
            rpusbdisp_disp_fillrect_packet_t fillRectPacket;
            
            fillRectPacket.header.cmd_flag = RPUSBDISP_DISPCMD_RECT;
            fillRectPacket.left = cpu_to_le16(left);
            fillRectPacket.top = cpu_to_le16(top);
            fillRectPacket.right = cpu_to_le16(right);
            fillRectPacket.bottom = cpu_to_le16(bottom);
            fillRectPacket.color_565 = color;
            fillRectPacket.operation = (_u8)bitOperation;
            
            sendCommandToDisplayEndpoint(fillRectPacket);
        }
        
        void copyArea(uint16_t srcX, uint16_t srcY, uint16_t destX, uint16_t destY, uint16_t width, uint16_t height) {
            rpusbdisp_disp_copyarea_packet_t packet;
            
            packet.header.cmd_flag = RPUSBDISP_DISPCMD_COPY_AREA;
            packet.sx = cpu_to_le16(srcX);
            packet.sy = cpu_to_le16(srcY);
            packet.dx = cpu_to_le16(destX);
            packet.dy = cpu_to_le16(destY);
            packet.width = cpu_to_le16(width);
            packet.height = cpu_to_le16(height);
            
            if (device_->getDevice()->getFirmwareVersion() < RP_USB_DISPLAY_MIN_VERSION_COPY_AREA_BUG_FIX) {
            sendCommandToDisplayEndpoint(packet, shared_ptr<Buffer>(new Buffer(1)));
            } else {
                sendCommandToDisplayEndpoint(packet);
            }
        }
        
        void enable() {
            call_once(statusThreadOnceFlag_, bind(&RoboPeakUsbDisplayDeviceImpl::doEnable_, this));
            
            this->fill(0xcb20u);
            this_thread::sleep_for(chrono::milliseconds(200));
            this->fill(0xcb20u);
        }
        
        int getWidth() const {
            return RoboPeakUsbDisplayDevice::ScreenWidth;
        }
        
        int getHeight() const {
            return RoboPeakUsbDisplayDevice::ScreenHeight;
        }
        
        bool isAlive() const {
            return working_.load();
        }
        
        shared_ptr<DeviceHandle> getDevice() {
            return device_;
        }
        
        static vector<shared_ptr<Device>> enumDevices() {
            return Context::defaultContext()->lookupDevices(RoboPeakUsbDisplayDevice::UsbDeviceVendorId, RoboPeakUsbDisplayDevice::UsbDeviceProductId);
        }
        
        static shared_ptr<Device> findFirstDevice() {
            shared_ptr<DeviceList> devices = Context::defaultContext()->getDeviceList();
            
            for (size_t i = 0; i < devices->count(); i++) {
                shared_ptr<Device> device = devices->getDevice(i);
                
                if (device->getVid() != RoboPeakUsbDisplayDevice::UsbDeviceVendorId)
                    continue;
                
                if (device->getPid() != RoboPeakUsbDisplayDevice::UsbDeviceProductId)
                    continue;
                
                return device;
            }
            
            return nullptr;
        }
        
        static shared_ptr<RoboPeakUsbDisplayDevice> openFirstDevice() {
            shared_ptr<Device> device = findFirstDevice();
            
            if (!device)
                return nullptr;
            
            return shared_ptr<RoboPeakUsbDisplayDevice>(new RoboPeakUsbDisplayDevice(device->open()));
        }
        
    private:
        void statusFetchingWorker_() {
            shared_ptr<Transfer> transfer = device_->allocTransfer(EndpointDirectionIn, EndpointTransferTypeInterrupt, RoboPeakUsbDisplayDevice::UsbDeviceStatusEndpoint);
            transfer->setTransferBuffer(shared_ptr<Buffer>(new Buffer(32)));

            while (working_.load()) {
                try {
                    transfer->submit();
                    transfer->waitForCompletion();
                } catch (Exception& exp) {
                    exp.printToConsole();
                    working_.store(false);
                    break;
                }
                
                switch (transfer->getStatus()) {
                    case rp::deps::libusbx_wrap::TransferStatusCompleted:
                    {
                        rpusbdisp_status_normal_packet_t status = decodeAs<rpusbdisp_status_normal_packet_t>(transfer);
                        
                        status.touch_x = (_s32)(le32_to_cpu((_u32)(status.touch_x)));
                        status.touch_y = (_s32)(le32_to_cpu((_u32)(status.touch_y)));
                        
                        {
                            lock_guard<mutex> guard(statusLock_);
                            status_ = status;
                            
                            if (statusUpdatedCallback_) {
                                statusUpdatedCallback_(status);
                            }
                        }
                        break;
                    }
                    case rp::deps::libusbx_wrap::TransferStatusStall:
                        device_->clearEndpointHalt(RoboPeakUsbDisplayDevice::UsbDeviceStatusEndpoint);
                        break;
                    default:
                        fprintf(stderr, "Unexpcected status: 0x%08X\n", (int)transfer->getStatus());
                        break;
                }
            }
        }
        
        void doEnable_() {
            lock_guard<mutex> guard(statusLock_);
            this->pipeline_ = Context::defaultContext()->summonPipeline();
            this->pipeline_->start();
            
            this->working_.store(true);
            this->statusFetchingThread_ = move(thread(bind(&RoboPeakUsbDisplayDeviceImpl::statusFetchingWorker_, this)));
        }
        
        once_flag statusThreadOnceFlag_;
        mutex statusLock_;
        rpusbdisp_status_normal_packet_t status_;
        thread statusFetchingThread_;
        atomic<bool> working_;
        
        function<void(const rpusbdisp_status_normal_packet_t&)> statusUpdatedCallback_;
        
        shared_ptr<DeviceHandle> device_;
        InterfaceScope interfaceScope_;
        shared_ptr<Pipeline> pipeline_;
        
        int maxPacketSize_;
    };
    
    RoboPeakUsbDisplayDevice::RoboPeakUsbDisplayDevice(shared_ptr<DeviceHandle> device) : impl_(new RoboPeakUsbDisplayDeviceImpl(device)) {}
    RoboPeakUsbDisplayDevice::~RoboPeakUsbDisplayDevice() {};
    
    void RoboPeakUsbDisplayDevice::setStatusUpdatedCallback(std::function<void (const rpusbdisp_status_normal_packet_t &)> callback) {
        impl_->setStatusUpdatedCallback(callback);
    }
    
    rpusbdisp_status_normal_packet_t RoboPeakUsbDisplayDevice::getStatus() {
        return impl_->getStatus();
    }
    
    void RoboPeakUsbDisplayDevice::fill(uint16_t color) {
        impl_->fill(color);
    }
    
    void RoboPeakUsbDisplayDevice::bitblt(uint16_t x, uint16_t y, uint16_t width, uint16_t height, RoboPeakUsbDisplayBitOperation bitOperation, void *buffer) {
        impl_->bitblt(x, y, width, height, bitOperation, buffer);
    }
    
    void RoboPeakUsbDisplayDevice::fillrect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t color, RoboPeakUsbDisplayBitOperation bitOperation) {
        impl_->fillrect(left, top, right, bottom, color, bitOperation);
    }
    
    void RoboPeakUsbDisplayDevice::copyArea(uint16_t srcX, uint16_t srcY, uint16_t destX, uint16_t destY, uint16_t width, uint16_t height) {
        impl_->copyArea(srcX, srcY, destX, destY, width, height);
    }
    
    void RoboPeakUsbDisplayDevice::enable() {
        impl_->enable();
    }
    
    int RoboPeakUsbDisplayDevice::getWidth() const {
        return impl_->getWidth();
    }
    
    int RoboPeakUsbDisplayDevice::getHeight() const {
        return impl_->getHeight();
    }
    
    bool RoboPeakUsbDisplayDevice::isAlive() {
        return impl_->isAlive();
    }
    
    shared_ptr<DeviceHandle> RoboPeakUsbDisplayDevice::getDevice() {
        return impl_->getDevice();
    }
    
    vector<shared_ptr<Device>> RoboPeakUsbDisplayDevice::enumDevices() {
        return RoboPeakUsbDisplayDeviceImpl::enumDevices();
    }
    
    shared_ptr<Device> RoboPeakUsbDisplayDevice::findFirstDevice() {
        return RoboPeakUsbDisplayDeviceImpl::findFirstDevice();
    }
    
    shared_ptr<RoboPeakUsbDisplayDevice> RoboPeakUsbDisplayDevice::openFirstDevice() {
        return RoboPeakUsbDisplayDeviceImpl::openFirstDevice();
    }
    
}}}
