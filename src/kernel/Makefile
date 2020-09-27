SUBDIRS := init

OBJDIR := $(OBJDIR)/$(notdir $(CURDIR))

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
