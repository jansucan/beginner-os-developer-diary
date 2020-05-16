AS      := nasm
ASFLAGS := -f bin

SRCS_PRIMARY := primary.asm
IMGS_PRIMARY := primary.bin

OBJDIR := $(OBJDIR)/$(notdir $(CURDIR))
VPATH  := $(OBJDIR)

$(IMGS_PRIMARY): $(SRCS_PRIMARY) | $(OBJDIR)
	$(AS) $(ASFLAGS) -o $(OBJDIR)/$@ $^

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
