SECONDARY_STAGE_INFO_INC := secondary_stage_info.inc

AS      := nasm
ASFLAGS := -f bin

IMGS := primary.bin secondary.bin

OBJDIR := $(OBJDIR)/$(notdir $(CURDIR))
VPATH  := $(OBJDIR)

.PHONY: all $(IMGS) clean

all: $(IMGS)

# The primary image is dependent on th secondary image because the
# primary stage needs to know how many 512B sectors the secondary
# stage occupies.
primary.bin: primary.asm $(SECONDARY_STAGE_INFO_INC) makefile.inc
	$(AS) $(ASFLAGS) -I $(OBJDIR)/ -o $(OBJDIR)/$@ $<

$(SECONDARY_STAGE_INFO_INC): secondary.bin
        # Find out the size of the secondary stage image
	echo SECONDARY_STAGE_SECTOR_COUNT equ $$( \
	  ls -l --block-size $(SECTOR_SIZE) $(OBJDIR)/$< | \
	  cut -d' ' -f5) \
	  > $(OBJDIR)/$(SECONDARY_STAGE_INFO_INC)
	# Find out an address the secondary stage is supposed to be
	# loaded at
	echo SECONDARY_STAGE_OFFSET equ $$( \
	  grep org secondary.asm | \
	  head -n 1 | \
          awk '{ print $$2 }') \
	  >> $(OBJDIR)/$(SECONDARY_STAGE_INFO_INC)

secondary.bin: secondary.asm makefile.inc
	$(AS) $(ASFLAGS) -I $(OBJDIR)/ -I$(OBJDIR)/../kernel/init \
	  -o $(OBJDIR)/$@ $<

makefile.inc: | $(OBJDIR)
	echo MAKEFILE_IMAGE_BOOTLOADER_SECONDARY_OFFSET equ \
	  $(IMAGE_BOOTLOADER_SECONDARY_OFFSET) \
	  > $(OBJDIR)/$@
	echo MAKEFILE_IMAGE_KERNEL_INIT_OFFSET equ \
	  $(IMAGE_KERNEL_INIT_OFFSET) \
	  >> $(OBJDIR)/$@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)
