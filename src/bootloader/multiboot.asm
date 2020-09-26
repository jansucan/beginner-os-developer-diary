MULTIBOOT_MAGIC_VALUE       equ 0x2BADB002
MULTIBOOT_SMAP		    equ 0x534D4150	
MULTIBOOT_MMAP_MAX_ENTRIES  equ 32
MULTIBOOT_MMAP_ENTRY_SIZE   equ 28


multiboot_struct_info:
multiboot_struct_info_flags:
	dd 0x00000040			; Only mmap_length and mmap_addr fields will pre present
	times 10 dd 0			; Unused entries
multiboot_struct_info_mmap_length:
	dd 0
multiboot_struct_info_mmap_addr:
	dd multiboot_mmap

multiboot_mmap:
	times (MULTIBOOT_MMAP_MAX_ENTRIES * MULTIBOOT_MMAP_ENTRY_SIZE) db 0


multiboot_detect_memory_map:
	mov dword [multiboot_struct_info_mmap_length],0
	mov dword [multiboot_struct_info_mmap_addr],multiboot_mmap
	mov di,multiboot_mmap + 4
	mov ebx,0
multiboot_detect_memory_map_next:
	mov eax,0xe820
	mov ecx,20
	mov edx,MULTIBOOT_SMAP
	int 0x15
	;; BIOS returned an address range descriptor
	jc error
	cmp eax,MULTIBOOT_SMAP
	jne error
	cmp ecx,20
	jl error
	;; At least 20 bytes have been returned
	add dword [multiboot_struct_info_mmap_length],24
	mov dword [di - 4],24
	add di,24
	;; Is this the last descriptor?
	cmp ebx,0
	jne multiboot_detect_memory_map_next
	;; Done
	mov ax,0
	ret
error:	mov ax,1
	ret
