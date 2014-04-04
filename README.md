RoboPeak Mini USB Display
====================================

Drivers and Tools for RoboPeak Mini USB Display Project

Visit RoboPeak Website for details. 

Demo Video
====================================
http://www.youtube.com/watch?v=KCNrq1hb99U

[国内用户:] http://www.tudou.com/programs/view/rJd1TwZzRZk/


How to Integrate the Driver in the Kernel build operation
=========================================================
    1) Copy the content of linux-driver folder in your kernel source (ideally in a new folder called robopeak inside the drivers/video folder)
    2) Replace the Makefile file by the NewMakefile one
    3) Edit the Kconfig file of the drivers/video file and insert the line
       * source "drivers/video/robopeak/Kconfig"
       after the line
       * comment "Frame buffer hardware drivers"
    4) Change the .config of your kernel through the menuconfig
       * make ARCH=your_architecture your_defconfig menuconfig
    5) In the menu "Device Drivers -> Graphic supports -> Support for frame buffer display" a Robopeak USB Display menu appears
    6) Set the Robopeak USB Display as module (this selection activates automatically the requested frame buffer option, see Prerequites of How to build the Linux Kernel Driver chapter)
    7) Generate your kernel


 

How to Build the Linux Kernel Driver
====================================
Here we only provide you the basic building process. Please refer to the related documents for details.


I. Prerequisite
-----------------
As a linux kernel driver, it requires you to provide the target system's kernel header or the full source code to build.

Before building the driver, you may need to config the linux kernel to enable the features required by the driver. Otherwise, the building process will fail.

If you want to use the driver with your current kernel (without recompiling the kernel and replacing it with the current one), please make sure the kernel header, the config and the related build scripts you used is belonged to this version of kernel.

Please make sure the following kernel features have been enabled. (via make menuconfig under your kernel source)
    
    0) framebuffer support (CONFIG_FB=y)
    
    1) deferred io support in framebuffer (CONFIG_FB_DEFERRED_IO=y)
    
    2) fb file operation support
       
       * CONFIG_FB_CFB_FILLRECT=y
       * CONFIG_FB_CFB_COPYAREA=y
       * CONFIG_FB_CFB_IMAGEBLIT=y
       * CONFIG_FB_SYS_FILLRECT=m
       * CONFIG_FB_SYS_COPYAREA=m
       * CONFIG_FB_SYS_IMAGEBLIT=m
       * CONFIG_FB_SYS_FOPS=m
       * CONFIG_FB_MODE_HELPERS=y
    
    3) Input event support (Generic input layer support)

You may modify the .config directly to enable these features.

Alternatively, here is a tricky method:
    Select the Displaylink display driver ( Device Drivers-> Graphics support -> Support for frame buffer devices-> Displaylink USB Framebuffer support) as an external module. By doing so, the above features will be selected by the menuconfiger.  
    
You may need to recompile the kernel if the above configuration changes have been made.

II. Native Build - build the kernel driver for the current machine
-----------------------------------------------------------------

You should have followed the steps described in the prerequisite section already. Here let's assume the location of the Linux kernel header(or source) is ~/workspace/linux-kernel.

Enter the Robopeak USB Displayer linux kernel driver folder (i.e. rpusbdisp/drivers/linux-driver), using the following command:

$ make KERNEL_SOURCE_DIR=~/workspace/linux-kernel

You should find the build result under the current folder: rp_usbdisplay.ko

III. Cross Compile - build the kernel driver for other platforms
------------------------------------------------------------------

You should have followed the steps described in the prerequisite section already. Here let's assume the location of the  Linux kernel header(or source) is ~/workspace/target-linux-kernel.

Let's also assume the target platform is ARMv7. We need to use the cross-compiler : arm-linux-gnueabihf-gcc.

Enter the following command to build the kernel driver for the target:
make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm KERNEL_SOURCE_DIR=~/workspace/target-linux-kernel

You should find the build result under the current folder: rp_usbdisplay.ko


How to Use the Kernel Driver
============================

I.deploy the dependencies modules
---------------------------------
If you had re-configed and recompiled the Linux kernel as required by the building process, you need to deploy the new kernel and all the kernel modules to the target system.

To be specific, you need to deploy AT LEAST the following kernel module to the  target system's /lib/module/<target_kernel_version> folder along with the new kenrel image:

  * sysfillrect.ko
  * syscopyarea.ko
  * sysimgblt.ko
  * fb_sys_fops.ko

II. deploy the compiled kernel driver
-------------------------------------

Reboot the target system to using the new kernel. Copy the compiled usb display driver (rp_usbdisplay.ko) to the following location:
    
    /lib/modules/`uname -r`/kernel

Enter the above folder and execute the following command:
    
    depmod -a

III. load the kernel driver
---------------------------
Once you had deployed the kernel driver and all of its dependencies, you can ask the kernel to load the driver using :
    
    modprobe rp_usbdisplay

By default the frame per seconds is set to 16. You can change it with the fps option when you load the driver :

    modprobe rp_usbdisplay fps=25

In this case the frame per seconds is set to 25.

If you want to let the kernel load the driver automatically each time when the system starts, you can added the following line into the file /etc/modules:
    rp_usbdisplay

(i.e. cat rp_usbdisplay >> /etc/modules)


IV. verify the driver
---------------------
Using the lsmod command to check whether the driver has been loaded correctlly. You should get the output similar to the following text:

  # lsmod
  Module                  Size  Used by
  rp_usbdisplay          12171  0 [permanent]
  fb_sys_fops             1412  1 rp_usbdisplay
  sysimgblt               2199  1 rp_usbdisplay
  sysfillrect             3295  1 rp_usbdisplay
  syscopyarea             3112  1 rp_usbdisplay

Also, you should find the following message in the dmesg log:

  [    7.535799] input: RoboPeakUSBDisplayTS as /devices/virtual/input/input0

  [    7.548115] usbcore: registered new interface driver rp-usbdisp

To verify the driver work with the RoboPeak USB Display, connect the display to the target system via the USB cable. The display should display RoboPea Logo and turn to black (or something else) for about 3 second.

If the display keeps showing the white noise animation and the message : Waiting for signal, you should check the dmesg to see what happens. Normally, the driver will prompt the following message when the display has been pluged in :

   [ 1814.173232] rp-usbdisp 4-1:1.0: RP USB Display found (#1), Firmware Version: X.XX, S/N: XXXXXXXXXXXX

Once the driver recognizes the display, a framebuffer device will be created. (e.g. /dev/fb0)

Use the following command to see whether framebuffer device is belonged to the USB display:

  # cat /proc/fb
  
  2 rpusbdisp-fb < 

In the above example, /dev/fb2 is the related framebuffer device. To test whether the framebuffer device works, you may using the following command:

  # cat /dev/urandom > /dev/fb2

You should see the display screen is filled with random color dots.

V. make X11 output to the USB display
-------------------------------------

There is a sample X11 config file under the source folder: rpusbdisp/drivers/linux-driver/xserver_conf/10-disp.conf. You can use this sample file as the template to make X11 on your target system to output to the USB display.

STEP1: determine the framebuffer device name of the display.
  use the command cat /proc/fb

STEP2: modify the 10-disp.conf, change the framebuffer device name to the one determined in STEP1.

STEP3: copy the file 10-disp.conf to the X11's config folder (/usr/share/X11/xorg.conf.d for most systems). 

STEP4: restart the X11 server

The X11 desktop should appear on the USB display.

Contact Us
====================================
Website: www.RoboPeak.com
Email:   support@robopeak.com
