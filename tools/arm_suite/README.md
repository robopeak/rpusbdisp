# Introduction #

RoboPeak USB Display Linux Driver Arm Suite is used to enable RoboPeak USB Display on ubuntu for arm (linaro) on arm SoCs.


# Install #

Use scp to copy `arm_suite.tar.bz2` to the host:

    $ scp arm_suite.tar.bz2 ubuntu@your_mini_pc/arm_suite.tar.bz2

And than connect to your host via ssh to install the driver:

    $ ssh -l ubuntu your_mini_pc
    $ tar xf arm_suite.tar.bz2
    $ cd arm_suite
    $ sudo ./rpusbdisp_arm_tool.sh install
    $ sudo ./rpusbdisp_arm_tool.sh auto_load

This will install prebuilt driver to the mini pc, as well as `lightdm` configuration and `upstart` configuration


# Usage #

Just reboot your mini pc by:

    $ sudo reboot

The driver will be loaded automatically on system startup

When you plug in your RoboPeak USB Display, the driver daemon will automatically configure `lightdm`, and restart it. This will also happen when you unplug your device.

* NOTE: Every time you plug in or pull out your RoboPeak USB Display, the driver daemon will restart `lightdm`, so your current GUI login session will be dropped *


# Homebrew #

If there is no prebuilt driver available for your device/kernel combination, you may build the kernel driver on your own, for detailed information please refer to our repository hosted on GitHub:

<https://github.com/robopeak/rpusbdisp>

# Copyright & Contact #

Copyright (c) 2013 RoboPeak.com
If you have any question or advice, please make us know. We will be very thankful if you contact us:
<mailto:support@robopeak.com>

