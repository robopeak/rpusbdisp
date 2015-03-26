#  
#  Unified Building System for Linux
#  By CSK (csk@live.com)
#  

PRJ_ROOT:=..
MOD_NAME:=rpusbdisp-drv

include $(PRJ_ROOT)/scripts/def.mak


CSRC:= $(shell find src -name "*.c")
CXXCCSRC:= $(shell find src -name "*.cc")



EXTRA_FLAGS:=-Wno-deprecated
CDEFS+=
DEP_AR+=$(OUTPUT_ROOT)/infra.a $(OUTPUT_ROOT)/usbxwrap.a $(OUTPUT_ROOT)/rpusbdisp-drv.a 

INCLUDES+= -Iinclude \
	   -I$(PRJ_ABS_ROOT)/deps-wraps/libusbx_wrap/include \
	   -I$(PREFIX)/include/libusb-1.0

all: $(DYNAMIC_DEST)
additional_clean:
	rm -f $(DYNAMIC_DEST)

install: $(DYNAMIC_DEST)
	install -m 0755 $(DYNAMIC_DEST) $(PREFIX)/lib
	tar cf - include | (cd $(PREFIX); tar xf -)

include $(PRJ_ROOT)/scripts/common.mak



