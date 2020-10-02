include file_size.mk

INIT_AS       := i686-elf-as
INIT_GCC      := i686-elf-gcc
INIT_READELF  := i686-elf-readelf
INIT_OBJCOPY  := i686-elf-objcopy
INIT_CFLAGS   := -std=gnu99 -ffreestanding -O2 -Wall -Wextra
INIT_LFLAGS   := -T init.ld -ffreestanding -O2 -nostdlib -lgcc

IMGS := init.bin

OBJDIR := $(OBJDIR)/$(notdir $(CURDIR))
VPATH  := $(OBJDIR)

.PHONY: all $(IMGS) clean

all: $(IMGS)

boot.o: boot.asm | $(OBJDIR)
	$(INIT_AS) -o $(OBJDIR)/$@ $<

%.o: %.c | $(OBJDIR)
	$(INIT_GCC) $(INIT_CFLAGS) -o $(OBJDIR)/$@ -c $<

init.bin: boot.o init.o multiboot.o terminal.o
	# Link to ELF format
	$(INIT_GCC) $(INIT_LFLAGS) -o $(OBJDIR)/$(@:%.bin=%.elf) $(addprefix $(OBJDIR)/,$(notdir $^))
	# Convert ELF to binary file for easy loading with the secondary stage bootloader
	$(INIT_OBJCOPY) -O binary $(OBJDIR)/$(@:%.bin=%.elf) $(OBJDIR)/$@
	# Get the lowest physical address of the loadable sections in the ELF file
	echo KERNEL_INIT_LOAD_OFFSET equ $$( \
	  $(INIT_READELF) -l $(OBJDIR)/$(@:%.bin=%.elf) | \
	  grep LOAD | \
	  sort -r -k4,4 | \
	  head -n 1 | \
          awk '{ print $$4 }' \
	  ) \
	  > $(OBJDIR)/info_kernel_init.inc
	# Get an address of the entry point
	echo KERNEL_INIT_ENTRY_OFFSET equ $$( \
	  $(INIT_READELF) -h $(OBJDIR)/$(@:%.bin=%.elf) | \
	  grep 'Entry point address' | \
	  head -n 1 | \
          awk '{ print $$4 }' \
	  ) \
	  >> $(OBJDIR)/info_kernel_init.inc
	# Get a size of the binary image in sectors
	echo KERNEL_INIT_SECTOR_COUNT equ \
	   $(call file_size_in_sectors,$(OBJDIR)/$@) \
	  >> $(OBJDIR)/info_kernel_init.inc

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)