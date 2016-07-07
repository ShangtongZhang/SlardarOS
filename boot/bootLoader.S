#include "bootLoader.h"

.text
.globl start
start:
.code16

xorl %eax, %eax
movw %ax, %ds
movw %ax, %es
movw %ax, %ss

# xorl %eax, %eax
# movw %cs, %ax
# shll $4, %eax
# lea protectModeCode, %ebx
# addl %ebx, %eax
lea protectModeCode, %eax

# xorl %ebx, %ebx
# movw %cs, %bx
# shll $4, %ebx
# lea gdt, %ecx
# addl %ecx, %ebx
lea gdt, %ebx

add $8, %ebx
add $2, %ebx
movw %ax, (%bx)		#ax -> gdt + 8 + 2
shrl $16, %eax
add $2, %bx 		#ax -> gdt + 8 + 4
movb %al, (%bx)
add $3, %bx			#ax -> gdt + 8 + 7
movb %ah, (%bx)

cli
inb $0x92, %al
orb $0x02, %al
outb %al, $0x92

lgdt gdtPtr
movl %cr0, %eax
orl $1, %eax
movl %eax, %cr0

ljmp $0x8, $0

protectModeCode:
.code32
call bootmain 

# .data
gdt:
	GDT_DESCRIPTOR(0, 0, 0)
	GDT_DESCRIPTOR(0, 0x100, GDT_CODE + GDT_32)
gdtPtr:
	.word 0x0f
	.long gdt
