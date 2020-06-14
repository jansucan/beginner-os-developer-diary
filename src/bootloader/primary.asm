	bits 16

%include "secondary_stage_info.inc"

STACK_SEGMENT	equ 0x0000
STACK_OFFSET	equ 0x7c00

        org 0x7c00
        jmp main

	msg_prefix db 'Primary stage: ',0
        msg_loaded db 'Loaded',0dh,0ah,0
	msg_loading_secondary db 'Loading secondary stage ... ',0
	msg_error db 'Error',0dh,0ah,0
	msg_ok db 'OK',0dh,0ah,0

main:
        ;; Move the stack to the area of RAM that is guaranteed free for use.
        mov ax,STACK_SEGMENT
        cli
        mov ss,ax
        mov sp,STACK_OFFSET
        sti
	mov si,msg_prefix
        call print
	mov si,msg_loaded
        call print
	;; Load the secondary stage
	mov si,msg_prefix
        call print
	mov si,msg_loading_secondary
	call print
	mov ah,02h
	mov al,SECONDARY_STAGE_SECTOR_COUNT
	mov ch,0
	mov cl,2
	mov dh,0
	;; DL register is already set by the BIOS to the boot drive number
	mov bx,SECONDARY_STAGE_OFFSET
	int 13h
	;; Check for error
	jc error
	mov si,msg_ok
	call print
	;; Execute the second stage
	jmp near SECONDARY_STAGE_OFFSET
error:
	mov si,msg_error
	call print
	cli
	hlt

%include "print.asm"

	;; Boot sector has to have 512 bytes. The last two bytes are the
	;; signature.
        times (510 - $ + $$) db 0h

	;; Boot sector signature.
	dw 0xaa55
