#  
#  Unified Building System for Linux
#  By CSK (csk@live.com)
#  

PRJ_ROOT:=..
MOD_NAME:=infra

include $(PRJ_ROOT)/scripts/def.mak


CSRC:= $(shell find src -name "*.c")
CXXCCSRC:= $(shell find src -name "*.cc")



EXTRA_FLAGS:=-Wno-deprecated
CDEFS+=
DEP_AR+= 

INCLUDES+= -Iinclude

all: $(STATIC_DEST)
additional_clean:
	rm -f $(STATIC_DEST)


install: 
	tar cf - include | (cd $(PREFIX); tar xf -)

include $(PRJ_ROOT)/scripts/common.mak



