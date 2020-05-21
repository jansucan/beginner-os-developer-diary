IMAGE_NAME    := boot.img
IMAGE_SIZE_MB := 8

SUBDIRS := bootloader
OBJDIR  := $(abspath ./build)

export OBJDIR

.PHONY: all $(SUBDIRS) image clean

all: $(SUBDIRS) image

$(SUBDIRS):
	$(MAKE) -C $@

image: $(SUBDIRS) | $(IMAGE_NAME)
	dd if=$(OBJDIR)/bootloader/primary.bin of=$(IMAGE_NAME) conv=notrunc
	dd if=$(OBJDIR)/bootloader/secondary.bin of=$(IMAGE_NAME) conv=notrunc \
	  bs=512 seek=1

clean:
	rm -rf $(OBJDIR) $(IMAGE_NAME)
	find . -name "*~" | xargs rm -f

$(IMAGE_NAME):
	dd if=/dev/zero of=$(IMAGE_NAME) bs=$$(( 1024 * 1024 )) \
	  count=$(IMAGE_SIZE_MB)
