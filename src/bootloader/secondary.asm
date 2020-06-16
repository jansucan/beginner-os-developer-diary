	bits 16

        org 0x7e00
        jmp main

%include "print.asm"
%include "a20_line.asm"
%include "nmi.asm"

	msg_prefix db 'Secondary stage: ',0
	msg_error db 'ERROR: ',0
        msg_a20_enabled db 'A20 is enabled',0dh,0ah,0
        msg_a20_disabled db 'A20 is disabled',0dh,0ah,0

	;; Global Descriptor Table for a flat memory setup
gdt_start:
	;; Null descriptor
	dd 0
	dd 0
	;; Code segment
	dw 0FFFFh 			; Limit (bits 0:15)
	dw 0 				; Base (bits 0:15)
	db 0 				; Base (bits 16:23)
	db 10011010b 			; Access byte
	db 11001111b 			; Flags and limit (bits 16:19)
	db 0 				; Base (bits 24:31)
	;; Data segment
	dw 0FFFFh 			; Limit (bits 0:15)
	dw 0 				; Base (bits 0:15)
	db 0 				; Base (bits 16:23)
	db 10010010b 			; Access byte
	db 11001111b 			; Flags and limit (bits 16:19)
	db 0 				; Base (bits 24:31)
gdt_end:

	;; Global Descriptor Table Register
gdtr:	dw gdt_end - gdt_start - 1 	; Limit (Size of the GDT)
	dd gdt_start 			; Base of the GDT

;;---------------------------------------------------------------------------

main:
	;; Check if A20 line is enabled
	mov si,msg_prefix
        call print
	mov si,msg_a20_enabled
	call a20_line_is_enabled
	cmp ax,1
	je main_a20
	mov si,msg_error
	call print
	mov si,msg_a20_disabled
main_a20:
	call print

	;; Disable non-maskable interrupt
	call nmi_disable

	;; Load GDT for a flat memory setup
	lgdt [gdtr]

	;; Enable protected mode
	mov eax,cr0
	or eax,1
	mov cr0,eax

	;; Clear prefetch queue
	jmp 0x08:main_clear_prefetch
main_clear_prefetch:

	;; 32-bit code in protected mode
	bits 32

	;; Reload data segment registers:
	mov   ax, 0x10 	; 0x10 points at the new data selector
	mov   ds, ax
	mov   es, ax
	mov   fs, ax
	mov   gs, ax
	mov   ss, ax

	hlt
