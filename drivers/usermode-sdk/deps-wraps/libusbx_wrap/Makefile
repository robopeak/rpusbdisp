#  
#  Unified Building System for Linux
#  By CSK (csk@live.com)
#  

PRJ_ROOT:=../..
MOD_NAME:=usbxwrap

include $(PRJ_ROOT)/scripts/def.mak


CSRC:= $(shell find src -name "*.c")
CXXCCSRC:= $(shell find src -name "*.cc")



EXTRA_FLAGS:=-Wno-deprecated
CDEFS+=
DEP_AR+= 

INCLUDES+= -Iinclude -I$(PREFIX)/include/libusb-1.0

all: $(STATIC_DEST)
additional_clean:
	rm -f $(STATIC_DEST)

include $(PRJ_ROOT)/scripts/common.mak



