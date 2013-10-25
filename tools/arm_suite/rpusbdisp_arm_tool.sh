#! /bin/bash

function install_kernel_module {
    echo "Installing kernel module..."
    echo "Please enter your board type (pcduino, cubieboard, cubieboard2...):"
    read
    BOARD_TYPE=$REPLY
    KERNEL_VERSION=$(uname -r)
    
    MODULE_PATH=kernel_modules/${BOARD_TYPE}/${KERNEL_VERSION}/rp_usbdisplay.ko

    if [ ! -f $MODULE_PATH ]; then
        echo "No suitable kernel module found for board '$BOARD_TYPE' and kernel version '$KERNEL_VERSION'"
        echo "Please contact support@robopeak.com to request your combination or compile it by yourself"
        exit 1
    fi

    INSTALL_PATH=/lib/modules/${KERNEL_VERSION}/kernel/rp_usbdisplay.ko

    if [ -e $INSTALL_PATH ]; then
        echo "File '$INSTALL_PATH' already exist, override it?"
        read
        if [ "$REPLY" != "yes" ]; then
            exit 1;
        fi
    fi

    echo "Installing"
    echo "  From: $MODULE_PATH"
    echo "  To: $INSTALL_PATH"
    install "$MODULE_PATH" "$INSTALL_PATH" || exit 1

    if [ ! -e $INSTALL_PATH ]; then
        echo "Failed to install kernel module"
        exit 1;
    fi

    depmod -a || exit 1
}

function auto_load {
    echo "Setting up rpusbdisp daemon..."
    
    mkdir -p /etc/rpusbdisp || exit 1
    cp scripts/rpusbdispd.sh /etc/rpusbdisp || exit 1
    cp scripts/rpusbdispd /etc/init.d || exit 1
    cp conf/10-disp.conf /etc/rpusbdisp || exit 1
    cp conf/rpusbdisp.conf /etc/init || exit 1
    initctl reload-configuration || exit 1

    echo "Done"
}

case "$1" in
    install_kernel_module)
        install_kernel_module
        ;;
    auto_load)
        auto_load
        ;;
    *)
        echo "Unknown command '$1'"
        echo "Supported commands:"
        echo "  install_kernel_module"
        echo "  auto_load"

        exit 1
        ;;
esac

