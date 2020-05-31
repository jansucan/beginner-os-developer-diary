	bits 16

        org 0x7e00
        jmp main

	msg_prefix db 'Secondary stage: ',0
        msg_a20_enabled db 'A20 is enabled',0dh,0ah,0
        msg_a20_disabled db 'A20 is disabled',0dh,0ah,0

main:
	mov si,msg_prefix
        call print
	mov si,msg_a20_enabled
	call a20_line_is_enabled
	cmp ax,1
	je main_a20
	mov si,msg_a20_disabled
main_a20:
	call print

	cli
	hlt

%include "print.asm"
%include "a20_line.asm"
