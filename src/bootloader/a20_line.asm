;; Bootsector signature original location address.
BOOTSEC_SIGNATURE_LOW_SEGMENT   equ 0x0000
BOOTSEC_SIGNATURE_LOW_OFFSET    equ 0x7DFE

;; The bootsector signature address + 1 MiB.
;; This is the wrap around address if A20 is enabled.
BOOTSEC_SIGNATURE_HIGH_SEGMENT  equ 0xFFFF
BOOTSEC_SIGNATURE_HIGH_OFFSET   equ 0x7E0E

;; Interrupts have to be disabled to prevent modification of the bootsector from an interrupt.
a20_line_is_enabled:
        pushf
        push    ds
        push    si
        push    es
        push    di

        mov     ax, BOOTSEC_SIGNATURE_LOW_SEGMENT
        mov     ds, ax
        mov     si, BOOTSEC_SIGNATURE_LOW_OFFSET

        mov     ax, BOOTSEC_SIGNATURE_HIGH_SEGMENT
        mov     es, ax
        mov     di, BOOTSEC_SIGNATURE_HIGH_OFFSET

        ;; Read the bootsector value using the wrap around address.
        mov     al, byte [es:di]
        ;; Change it.
        not     al
        ;; Write it to the bootsector using its original location address.
        mov     byte [ds:si], al
        ;; Check if the changed value can be read using the wrap around address.
        cmp     al, byte [es:di]

        ;; Restore the original byte of the bootsector.
        ;; NOT and MOV instructions does not affect the flags (set by CMP).
        not     al
        mov     byte [ds:si], al

        mov     ax, 0
        je      a20_line_is_enabled_exit

        mov     ax, 1

a20_line_is_enabled_exit:
        pop     di
        pop     es
        pop     si
        pop     ds
        popf
        ret
