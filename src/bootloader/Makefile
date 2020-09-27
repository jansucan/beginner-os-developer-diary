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
primary.bin: primary.asm info_secondary_stage.inc makefile.inc
	$(AS) $(ASFLAGS) -I $(OBJDIR)/ -o $(OBJDIR)/$@ $<

info_secondary_stage.inc: secondary.bin
        # Find out the size of the secondary stage image
	echo SECONDARY_STAGE_SECTOR_COUNT equ $$( \
	  ls -l --block-size $(SECTOR_SIZE) $(OBJDIR)/$< | \
	  cut -d' ' -f5) \
	  > $(OBJDIR)/$@
	# Find out an address the secondary stage is supposed to be
	# loaded at
	echo SECONDARY_STAGE_OFFSET equ $$( \
	  grep org secondary.asm | \
	  head -n 1 | \
          awk '{ print $$2 }') \
	  >> $(OBJDIR)/$@

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
