#  VFD Demo System
#  By CSK
#


# NOTE: do not alter the order of the folders, they are arranged based on the dependencies
SUBDIRS:= libusbx_wrap
	


.PHONY: all clean install listsubs

listsubs:
	@for subdir in $(SUBDIRS) ; do $(MAKE) -C $$subdir $(MAKECMDGOALS) || exit 1; done

all: listsubs

clean: listsubs

install: listsubs

