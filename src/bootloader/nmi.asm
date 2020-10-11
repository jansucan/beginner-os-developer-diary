nmi_disable:
        ;; NMI is disabled by setting bit 7 in CMOS RAM index register on port 0x70
        in      al, 0x70
        or      al, 0x80
        out     0x70, al
        ret
