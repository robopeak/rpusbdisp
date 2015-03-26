#  
#  Unified Building System for Linux
#  By CSK (csk@live.com)
#  

PRJ_ROOT:=..
MOD_NAME:=rpusbdispdemo

include $(PRJ_ROOT)/scripts/def.mak


CSRC = $(shell find . -name "*.c")
CXXCCSRC= $(shell find . -name "*.cc")

EXTRA_FLAGS:=-Wno-deprecated
CDEFS+= 
DEP_AR+=

DEP_LIBS+= $(RPUSBDISP_LIBS)

INCLUDES+= 


all: $(EXEC_DEST)
additional_clean:
	rm -f $(EXEC_DEST)

include $(PRJ_ROOT)/scripts/common.mak



