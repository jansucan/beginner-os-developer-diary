define file_size_in_sectors
	$$(ls -l --block-size $(SECTOR_SIZE) $(1) | cut -d' ' -f5)
endef
