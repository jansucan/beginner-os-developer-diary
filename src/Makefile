SECTOR_SIZE := 512

IMAGE_NAME    := boot.img
IMAGE_SIZE_MB := 8
IMAGE_BOOTLOADER_SECONDARY_OFFSET := 1
IMAGE_BOOTLOADER_SECONDARY_SPACE  := 4
IMAGE_KERNEL_INIT_OFFSET          := 5
IMAGE_KERNEL_INIT_SPACE           := 32

VIRTUAL_MACHINE       := qemu-system-x86_64 -device piix3-usb-uhci \
                         -drive id=my_usb_disk,file=$(IMAGE_NAME),if=none,format=raw \
                         -device usb-storage,drive=my_usb_disk
VIRTUAL_MACHINE_DEBUG := $(VIRTUAL_MACHINE) -gdb tcp::1234 -S

SUBDIRS       := bootloader kernel
OBJDIR        := $(abspath ./build)
MAKE_INCLUDES := $(abspath ./make_includes)


export IMAGE_BOOTLOADER_SECONDARY_OFFSET
export IMAGE_BOOTLOADER_SECONDARY_SPACE
export IMAGE_KERNEL_INIT_OFFSET
export IMAGE_KERNEL_INIT_SPACE
export OBJDIR
export SECTOR_SIZE


.PHONY: all $(SUBDIRS) image clean run run-debug

all: image

run-debug: image
run: image

bootloader: kernel

$(SUBDIRS):
	$(MAKE) -I $(MAKE_INCLUDES) -C $@

image: $(SUBDIRS) | $(IMAGE_NAME)
	dd if=$(OBJDIR)/bootloader/primary.bin of=$(IMAGE_NAME) conv=notrunc
	dd if=$(OBJDIR)/bootloader/secondary.bin of=$(IMAGE_NAME) conv=notrunc \
	  bs=$(SECTOR_SIZE) seek=$(IMAGE_BOOTLOADER_SECONDARY_OFFSET)
	dd if=$(OBJDIR)/kernel/init/init.bin of=$(IMAGE_NAME) conv=notrunc \
	  bs=$(SECTOR_SIZE) seek=$(IMAGE_KERNEL_INIT_OFFSET)

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
