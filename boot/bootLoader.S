#include "bootLoader.h"

.text
.globl start
start:
.code16

xorl %eax, %eax
movw %ax, %ds
movw %ax, %es
movw %ax, %ss

lea protectModeCode, %eax
lea gdt, %ebx

add $8, %ebx
add $2, %ebx
movw %ax, (%bx)		# -> gdt + 8 + 2
shrl $16, %eax
add $2, %bx 		# -> gdt + 8 + 4
movb %al, (%bx)
add $3, %bx			# -> gdt + 8 + 7
movb %ah, (%bx)

cli
inb $0x92, %al
orb $0x02, %al
outb %al, $0x92

lgdt gdtPtr

movl %cr0, %eax
orl $1, %eax
movl %eax, %cr0

movw $SELECTOR_DATA_SEG, %ax
mov %ax, %ds
xorl %esi, %esi
mov $SELECTOR_STACK_SEG, %ax
mov %ax, %ss
movl $TOP_OF_STACK, %eax
movl %eax, %esp
movl %eax, %ebp
ljmp $SELECTOR_CODE_SEG, $0

protectModeCode:
.code32
pushw $93
call bootmain
popw %ax

gdt:
	GDT_DESCRIPTOR(0, 0, 0)
	GDT_DESCRIPTOR(0, 0xfff, GDT_CODE + GDT_32)		# code seg, set base addr in runtime
	GDT_DESCRIPTOR(0x7000, 0xfff, GDT_DATA_RW)		# data seg
	GDT_DESCRIPTOR(0x8000, 0xfff, GDT_DATA_RW)		# stack seg
gdtPtr:
	.word 0x1f
	.long gdt
