#! /bin/bash

CONF_FILE_SOURCE=/etc/rpusbdisp/10-disp.conf
CONF_FILE_TARGET=/usr/share/X11/xorg.conf.d/10-disp.conf

function ensure_conf_exist {
    if [ ! -f "$CONF_FILE_TARGET" ]; then
        echo "Config doesn't exist creating one..."
        /etc/init.d/lightdm stop
        cp "$CONF_FILE_SOURCE" "$CONF_FILE_TARGET"
    fi
}

function ensure_conf_not_exist {
    if [ -f "$CONF_FILE_TARGET" ]; then
        echo "Removing disp conf..."
        /etc/init.d/lightdm stop
        rm -f "$CONF_FILE_TARGET"
    fi
}

function ensure_fb_id {
    RP_USB_DISP_FB_ID=$1
    HAS_CORRECT_FB_ID=$(cat $CONF_FILE_TARGET | grep /dev/fb${RP_USB_DISP_FB_ID})
    if [ "$HAS_CORRECT_FB_ID" == "" ]; then
        echo "fb id error, correcting fb id to $RP_USB_DISP_FB_ID..."
        /etc/init.d/lightdm stop
        cat "$CONF_FILE_SOURCE" | sed -E "s/\/dev\/fb[0-9]+/\/dev\/fb${RP_USB_DISP_FB_ID}/g" > "$CONF_FILE_TARGET"
    fi
}

function ensure_lightdm_running {
    LIGHT_DM_RUNNING=$(ps -A | grep lightdm | tail -n 1)
    if [ "${LIGHT_DM_RUNNING}" == "" ]; then
        echo "lightdm stopped, starting..."
        /etc/init.d/lightdm start
    fi
}

function daemon {
    while [ -e /tmp/rpusbdispd.run ]; do
        RP_USB_DISP_FB_ID=$(cat /proc/fb | grep rp | sed -E 's/^([0-9]+)\s.*$/\1/')
        STATUS=$(dmesg | grep "RP USB Display" | tail -n 1 | grep "found") 
        if [ "$STATUS" != "" ]; then
            echo "Rp usb display detected: $RP_USB_DISP_FB_ID"
            ensure_conf_exist
            ensure_fb_id $RP_USB_DISP_FB_ID
        else
            echo "No rp usb display detected"
            ensure_conf_not_exist
        fi
        ensure_lightdm_running
        sleep 1
    done
}


function start {
    if [ -e /tmp/rpusbdispd.run ]; then
        echo "rpusbdispd already running"
        exit 1
    fi

    KERNEL_MODULE_LOADED=$(lsmod | grep rp_usbdisplay)
    
    if [ "$KERNEL_MODULE_LOADED" == "" ]; then
        modprobe -f rp_usbdisplay
    fi

    touch /tmp/rpusbdispd.run
    daemon
}

function stop {
    if [ ! -e /tmp/rpusbdispd.run ]; then
        echo "rpusbdispd is not running"
        exit 1
    fi

    rm -rf /tmp/rpusbdispd.run
}

case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    *)
        echo "rpusbdispd.sh [start|stop]"
        exit 1
        ;;
esac

