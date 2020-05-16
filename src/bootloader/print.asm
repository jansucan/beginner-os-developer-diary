print:
        lodsb
	;; Test whether the loaded character is the terminating \0.
        or    al,al
        jz    short print_exit
        mov   ah,0eh
        mov   bx,000fh
        int   10h
        jmp   print
print_exit:
        ret
