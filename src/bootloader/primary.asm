	bits 16

STACK_SEGMENT	equ 0x8000
STACK_OFFSET	equ 0x0000

        org 0x7c00
        jmp main

        msg_hello db 'Hello, World!',0

main:
        ;; Move the stack to the area of RAM that is guaranteed free for use.
        mov ax,STACK_SEGMENT
        cli
        mov ss,ax
        mov sp,STACK_OFFSET
        sti
	mov si,msg_hello
        call print
	cli
	hlt

%include "print.asm"

	;; Boot sector has to have 512 bytes. The last two bytes are the
	;; signature.
        times (510 - $ + $$) db 0h

	;; Boot sector signature.
	dw 0xaa55
