#  
#  Unified Building System for Linux
#  By CSK (csk@live.com)
#  

include $(PRJ_ROOT)/Makefile.conf

.PHONY: all clean install distclean listsubs

# Path
PRJ_ABS_ROOT:=$(shell cd $(PRJ_ROOT) && pwd)

OBJ_PARENT_ROOT:=$(PRJ_ROOT)/build/obj/$(ARCH_PLATFORM)
OBJ_ROOT:=$(OBJ_PARENT_ROOT)/$(MOD_NAME)
OUTPUT_ROOT:=$(PRJ_ROOT)/build/output/$(ARCH_PLATFORM)

DEP_FILE += $(patsubst %.o, %.d, $(OBJ))


EXEC_FILENAME:=$(MOD_NAME)
EXEC_DEST:=$(OUTPUT_ROOT)/$(EXEC_FILENAME)

DYNAMIC_FILENAME:=lib$(MOD_NAME).so
DYNAMIC_DEST:=$(OUTPUT_ROOT)/$(DYNAMIC_FILENAME)

STATIC_FILENAME:=$(MOD_NAME).a
STATIC_DEST:=$(OUTPUT_ROOT)/$(STATIC_FILENAME)


# Building Flags

ifeq ($(OPT_DBG), yes)
OPT_LVL?=-g
CDEFS+= -D_DEBUG
else
OPT_LVL?=-O2
endif


ifeq ($(ARCH_PLATFORM), armv7l)
TARGET_DEF:= 
else
TARGET_DEF:= 
endif

CDEFS+= -DTARGET_$(ARCH_PLATFORM) 



# Dependencies
INCLUDES+= -I. \
	   -I$(PRJ_ABS_ROOT) \
	   -I$(PRJ_ABS_ROOT)/infra/include \
	   -I$(PRJ_ABS_ROOT)/rpusbdisp-drv/include \
	   -I$(PRJ_ABS_ROOT)/deps-wraps/libusbx_wrap/include \
	   -I$(PREFIX)/include

RPUSBDISP_LIBS:=-L$(OUTPUT_ROOT) -lrpusbdisp-drv

DEP_LIBS+=  -L$(PREFIX)/lib -lm \
	    -ldl \
	    -lpthread \
	    -lstdc++ \
	    -lrt \
	    -lusb-1.0



CFLAGS+= $(CDEFS) $(OPT_LVL) $(INCLUDES) $(TARGET_DEF) $(EXTRA_FLAGS) -Wconversion-null
CXXFLAGS+= $(CFLAGS) -std=c++11

LDFLAGS+= $(DEP_LIBS) -Wl,-rpath-link=$(PREFIX)/lib

