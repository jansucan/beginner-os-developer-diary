	bits 16

        org 0x7e00
        jmp main

	msg_prefix db 'Secondary stage: ',0
        msg_hello db 'Hello, World!',0dh,0ah,0

main:
	mov si,msg_prefix
        call print
	mov si,msg_hello
        call print
	cli
	hlt

%include "print.asm"
