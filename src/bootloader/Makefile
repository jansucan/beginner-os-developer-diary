SECTOR_SIZE := 512

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
primary.bin: primary.asm $(SECONDARY_STAGE_INFO_INC)
	$(AS) $(ASFLAGS) -I $(OBJDIR)/ -o $(OBJDIR)/$@ $<

$(SECONDARY_STAGE_INFO_INC): secondary.bin
        # Find out the size of the secondary stage image
	echo SECONDARY_STAGE_SECTOR_COUNT equ $$( \
	  ls --block-size $(SECTOR_SIZE) -s $(OBJDIR)/$< | \
	  cut -d' ' -f1) \
	  > $(OBJDIR)/$(SECONDARY_STAGE_INFO_INC)
	# Find out an address the secondary stage is supposed to be
	# loaded at
	echo SECONDARY_STAGE_OFFSET equ $$( \
	  grep org secondary.asm | \
	  head -n 1 | \
          awk '{ print $$2 }') \
	  >> $(OBJDIR)/$(SECONDARY_STAGE_INFO_INC)

secondary.bin: secondary.asm | $(OBJDIR)
	$(AS) $(ASFLAGS) -I$(OBJDIR)/../kernel/ -o $(OBJDIR)/$@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)
