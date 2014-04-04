//
//  rpusbdisp.h
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/11/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

/*! \mainpage RoboPeak Mini USB Display Usermode SDK
 *
 * \section intro Introduction
 * RoboPeak Mini USB Display is a low cost display module with USB connectivity for data
 * transmission and designed by RoboPeak Team. It can be convenient used as a Human
 * Interactive Interface device for various embedded devices and platforms.
 *
 * \section build Build SDK
 * For Windows and OS X users, you can just open the relative projects in usermode-sdk/workspace, and use Control-Shift-B or Command-B to build the SDK
 *
 * For Linux users, you need to install libusbx first, or build within libusbx
 * $ cd deps/libusbx-1.0.17
 * $ ./configure && make
 * Then just use make to build the library and builtin demo
 * make
 *
 * \section cpp_api C++ API
 * The SDK is written in morden C++, using a lot of C++ 11 features.
 * The C++ interface of RoboPeak USB Display SDK is very friendly and easy to use
 * Please view HERE for reference, and you can have a quick view of the interface by reading the cPlusPlusInterfaceDemo function in usermode-sdk/demo/main.cc
 *
 * \section c_api C API
 * The SDK also provide a C API, which is a thin wrap layer of the C++ API
 * Here are some references, and you can read the cInterfaceDemo function in usermode-sdk/demo/main.cc for the usage
 */

#include <memory>
#include <vector>
#include <future>
#include <rp/util/noncopyable.h>
#include <rp/util/int_types.h>
#include <rp/drivers/display/rpusbdisp/protocol.h>
#include <rp/drivers/display/rpusbdisp/enums.h>

namespace rp { namespace deps { namespace libusbx_wrap {
    
    class Device;
    class DeviceHandle;
    
}}}

namespace rp { namespace drivers { namespace display {
    
    class RoboPeakUsbDisplayDeviceImpl;
    
    /**
     * \brief RoboPeak Usb Display Device
     *
     * The abstract of all operation on RoboPeak Mini Usb Display devices.
     */
    class RoboPeakUsbDisplayDevice : public rp::util::noncopyable {
    public:
        /**
         * \brief Create a RoboPeak Usb Display Device
         *
         * DO NOT CALL THIS METHOD DIRECTLY, PLEASE USE THE openDevice STATIC METHOD OF THIS CLASS
         *
         * \param device The raw USB device from libusb, please use RoboPeakUsbDisplayDevice::enumDevices and relative APIs to get the device object
         */
        RoboPeakUsbDisplayDevice(std::shared_ptr<rp::deps::libusbx_wrap::DeviceHandle> device);
        ~RoboPeakUsbDisplayDevice();
        
        static const uint16_t UsbDeviceVendorId;
        static const uint16_t UsbDeviceProductId;
        
        static const uint8_t UsbDeviceStatusEndpoint;
        static const uint8_t UsbDeviceDisplayEndpoint;
        
        static const int ScreenWidth;
        static const int ScreenHeight;
        
        /**
         * \brief Set the callback to be called when the status of the display is updated
         *
         * \param callback The callback
         */
        void setStatusUpdatedCallback(std::function<void(const rpusbdisp_status_normal_packet_t&)> callback);
        
        /**
         * \brief Get device status
         */
        rpusbdisp_status_normal_packet_t getStatus();
        
        /**
         * \brief Fill the whole screen with color
         *
         * \param color A 16 bit color represent in B5G6R5 format
         */
        void fill(uint16_t color);
        
        /**
         * \brief Draw image to the display
         *
         * \param x The x coordinate where the image will be painted
         * \param y The y coordinate where the image will be painted
         * \param width The width of the image
         * \param height The height of the image
         * \param bitOperation The pixel bit operation will be done between the original pixel and the pixel from the image
         * \param buffer The buffer of the image, should be more than (width*height*2) bytes, and each pixel should be in B5G6B5 pixel format
         */
        void bitblt(uint16_t x, uint16_t y, uint16_t width, uint16_t height, RoboPeakUsbDisplayBitOperation bitOperation, void* buffer);
        
        /**
         * \brief Fill a rectangle of the display with a solid color
         *
         * NOTICE: The four edges are included in the fill operation
         *
         * \param left The left boundry of the rectangle
         * \param top The top boundry of the rectangle
         * \param right The right boundry of the rectangle
         * \param bottom The bottom boundry of the rectangle
         * \param bitOperation The pixel bit operation will be done when filling the rectangle
         */
        void fillrect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t color, RoboPeakUsbDisplayBitOperation bitOperation);
        
        /**
         * \brief Copy a part of the existing image of the screen to another position of the display
         *
         * \param srcX Source x coordinate
         * \param srcY Source y coordinate
         * \param destX Destination x coordinate
         * \param destY Destination y coordinate
         * \param width Width of the copying area
         * \param height Height of the copying area
         */
        void copyArea(uint16_t srcX, uint16_t srcY, uint16_t destX, uint16_t destY, uint16_t width, uint16_t height);
        
        /**
         * \brief Enable the device
         *
         * The device polling process is off before this method is invoked, so before you use the device, you should call this method to take the polling process online.
         */
        void enable();
        
        /**
         * \brief The width of the display (in pixel)
         */
        int getWidth() const;
        
        /**
         * \brief The height of the display (in pixel)
         */
        int getHeight() const;
        
        /**
         * \brief Indicate if the device is alive and healthy
         */
        bool isAlive();
        
        /**
         * \brief Get the inner raw usb device
         */
        std::shared_ptr<rp::deps::libusbx_wrap::DeviceHandle> getDevice();
        
        /**
         * \brief Enumerate all USB devices that match the VID and PID of USB Display
         */
        static std::vector<std::shared_ptr<rp::deps::libusbx_wrap::Device> > enumDevices();
        
        /**
         * \brief Find first device that has the required VID and PID
         */
        static std::shared_ptr<rp::deps::libusbx_wrap::Device> findFirstDevice();
        
        /**
         * \brief Open the first device that match the requirement
         */
        static std::shared_ptr<RoboPeakUsbDisplayDevice> openFirstDevice();
        
    private:
        std::shared_ptr<RoboPeakUsbDisplayDeviceImpl> impl_;
    };

}}}
