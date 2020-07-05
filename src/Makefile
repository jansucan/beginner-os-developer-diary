IMAGE_NAME    := boot.img
IMAGE_SIZE_MB := 8

VIRTUAL_MACHINE       := qemu-system-x86_64 -device piix3-usb-uhci \
                         -drive id=my_usb_disk,file=$(IMAGE_NAME),if=none,format=raw \
                         -device usb-storage,drive=my_usb_disk
VIRTUAL_MACHINE_DEBUG := $(VIRTUAL_MACHINE) -gdb tcp::1234 -S

SUBDIRS := bootloader kernel
OBJDIR  := $(abspath ./build)

export OBJDIR

.PHONY: all $(SUBDIRS) image clean run run-debug

all: image

run-debug: image
run: image

bootloader: kernel

$(SUBDIRS):
	$(MAKE) -C $@

image: $(SUBDIRS) | $(IMAGE_NAME)
	dd if=$(OBJDIR)/bootloader/primary.bin of=$(IMAGE_NAME) conv=notrunc
	dd if=$(OBJDIR)/bootloader/secondary.bin of=$(IMAGE_NAME) conv=notrunc \
	  bs=512 seek=1
	dd if=$(OBJDIR)/kernel/init.bin of=$(IMAGE_NAME) conv=notrunc \
	  bs=512 seek=3

clean:
	rm -rf $(OBJDIR) $(IMAGE_NAME)
	find . -name "*~" | xargs rm -f

$(IMAGE_NAME):
	dd if=/dev/zero of=$(IMAGE_NAME) bs=$$(( 1024 * 1024 )) \
	  count=$(IMAGE_SIZE_MB)

run:
	$(VIRTUAL_MACHINE)

run-debug:
	$(VIRTUAL_MACHINE_DEBUG)
