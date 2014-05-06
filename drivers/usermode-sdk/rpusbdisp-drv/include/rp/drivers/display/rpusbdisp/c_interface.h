//
//  c_interface.h
//  C interface for RoboPeak Mini USB Display sdk
//
//  Created by Tony Huang on 12/14/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <rp/infra_config.h>
#include <rp/util/int_types.h>
#include <rp/drivers/display/rpusbdisp/protocol.h>
#include <rp/drivers/display/rpusbdisp/enums.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    // Type definitions
    
    /**
     * \brief The return type of APIs
     *
     * All APIs of the C interface return the execution result in this type. You should use RoboPeakUsbDisplayDriverIsSuccess function to indicate if the operation is successful
     */
    typedef int RoboPeakUsbDisplayDriverResult;

    struct rp_usbdisp_device;

    /**
     * \brief A reference to a RoboPeak Usb Display Device
     */
    typedef struct rp_usbdisp_device* RoboPeakUsbDisplayDeviceRef;
    
    /**
     * \brief The type of callbacks will be triggered when the status of the display is updated
     */
    typedef void RoboPeakUsbDisplayStatusCallback(rpusbdisp_status_normal_packet_t*, void*);
    
    // Result operations
    /**
     * \brief A helper routine to indicate if an API call is successful
     */
    static inline bool RoboPeakUsbDisplayDriverIsSuccess(RoboPeakUsbDisplayDriverResult result) {
        return result == 0;
    }

    // Operations
    /**
     * \brief Open the first RoboPeak USB Display
     *
     * \param outDevice [out] A pointer to RoboPeakUsbDisplayDeviceRef to store the open device
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayOpenFirstDevice(RoboPeakUsbDisplayDeviceRef* outDevice);
    
    /**
     * \brief Dispose open device
     *
     * \param device The device to dispose
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayDisposeDevice(RoboPeakUsbDisplayDeviceRef device);
    
    /**
     * \brief Set the callback to be called when the display status is updated
     *
     * \param device The device to monitor
     * \param callback The callback
     * \param closure The user data used to fill the second argument of callback
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplaySetStatusUpdatedCallback(RoboPeakUsbDisplayDeviceRef device, RoboPeakUsbDisplayStatusCallback callback, void* closure);
    
    /**
     * \brief Fill the whole screen with color
     *
     * \param device The display device
     * \param color The color to be used (in B5G6R5 format)
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayFill(RoboPeakUsbDisplayDeviceRef device, uint16_t color);
    
    /**
     * \brief Draw an image to the screen
     *
     * \param device The display device
     * \param x The x coordinate where the image will be painted
     * \param y The y coordinate where the image will be painted
     * \param width The width of the image
     * \param height The height of the image
     * \param bitOperation The pixel bit operation will be done between the original pixel and the pixel from the image
     * \param buffer The buffer of the image, should be more than (width*height*2) bytes, and each pixel should be in B5G6B5 pixel format
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayBitblt(RoboPeakUsbDisplayDeviceRef device, uint16_t x, uint16_t y, uint16_t width, uint16_t height, RoboPeakUsbDisplayBitOperation bitOperation, void* buffer);
    
    /**
     * \brief Fill a rectangle of the display with a solid color
     *
     * NOTICE: The four edges are included in the fill operation
     *
     * \param device The display device
     * \param left The left boundry of the rectangle
     * \param top The top boundry of the rectangle
     * \param right The right boundry of the rectangle
     * \param bottom The bottom boundry of the rectangle
     * \param bitOperation The pixel bit operation will be done when filling the rectangle
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayFillRect(RoboPeakUsbDisplayDeviceRef device, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t color, RoboPeakUsbDisplayBitOperation bitOperation);
    
    /**
     * \brief Copy a part of the existing image of the screen to another position of the display
     *
     * \param device The display device
     * \param srcX Source x coordinate
     * \param srcY Source y coordinate
     * \param destX Destination x coordinate
     * \param destY Destination y coordinate
     * \param width Width of the copying area
     * \param height Height of the copying area
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayCopyArea(RoboPeakUsbDisplayDeviceRef device, uint16_t srcX, uint16_t srcY, uint16_t destX, uint16_t destY, uint16_t width, uint16_t height);
    
    
    /**
     * \brief Enable the device
     *
     * The device polling process is off before this method is invoked, so before you use the device, you should call this method to take the polling process online.
     *
     * \param device The display device
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayEnable(RoboPeakUsbDisplayDeviceRef device);
    
    /**
     * \brief Get the resolution of the screen
     *
     * \param device The display device
     * \param outWidth [out] The screen width
     * \param outHeight [out] The screen height
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayGetResolution(RoboPeakUsbDisplayDeviceRef device, int* outWidth, int* outHeight);
    
    /**
     * \brief Indicate if the device is alive and healthy
     *
     * \param device The display device
     */
	extern RP_INFRA_API RoboPeakUsbDisplayDriverResult RoboPeakUsbDisplayIsAlive(RoboPeakUsbDisplayDeviceRef device, bool* outAlive);

#ifdef __cplusplus
}
#endif
