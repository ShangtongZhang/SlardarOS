.globl start
start:
.code16
movw 0xa, %ax
.code32
call _bootmain 
