#include "bootLoader.h"

.text
.globl start
start:
.code16
	xorl %eax, %eax
	# movw %ax, %ds
	# movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movl $0x500, %eax
	movl %eax, %esp
	movl %eax, %ebp
	movw $0x600, %ax
	movw %ax, %es

	movw $19, sectorNo
	xorb %ah, %ah
	xorb %dl, %dl
	int $0x13

searchInRootDirBegin:
	movw rootDirSizeForLoop, %ax
	cmpw $0, %ax
	jz kernelNotFound
	decw %ax
	movw %ax, rootDirSizeForLoop
	movw $BASE_OF_KERNEL, %ax
	movw %ax, %es
	movw $OFFSET_OF_KERNEL, %ax
	movw %ax, %bx
	movw sectorNo, %ax
	movb $1, %cl
	call readSector
	
	lea kernelFileName, %si
	movw $OFFSET_OF_KERNEL, %di
	cld
	movw $0x10, %dx
searchForKernelBin:
	cmpw $0, %dx
	jz nextSectorInRootDir
	decw %dx
	movw $11, %cx
compareFileName:
	cmpw $0, %cx
	jz fileNameFound
	decw %cx
	lodsb
	movb %es:(%di), %bl 
	cmpb %al, %bl
	jz compareFileNameGoOn
	jmp fileNameDifferent
compareFileNameGoOn:
	inc %di
	jmp compareFileName

fileNameDifferent:
	and $0x0ffe0, %di
	add $0x20, %di
	lea kernelFileName, %si
	jmp searchForKernelBin

nextSectorInRootDir:
	movw sectorNo, %ax
	decw %ax
	movw %ax, sectorNo
	jmp searchInRootDirBegin

kernelNotFound:
	jmp kernelNotFound

fileNameFound:
	movl $ROOT_DIR_SECTORS, %eax
	andw $0xfff0, %di
	xorl %ebx, %ebx
	movw %es, %bx
	shll $4, %ebx
	addw %di, %bx
	addw $0x1c, %bx
	movw (%bx), %dx
	movw %dx, kernelSize
	addw $0x1a, %di
	movw %es:(%di), %cx
	pushw %cx
	addw %ax, %cx
	addw $DELTA_SECTOR_NUM, %cx
	movw $BASE_OF_KERNEL, %ax
	movw %ax, %es
	movw $OFFSET_OF_KERNEL, %bx
	movw %cx, %ax

goOnLoadingFile:
	movb $1, %cl
	call readSector
	popw %ax
	call getFATEntry
	cmpw $0xfff, %ax
	jz fileLoaded
	push %ax
	movw $ROOT_DIR_SECTORS, %dx
	addw %dx, %ax
	addw $DELTA_SECTOR_NUM, %ax
	addw $BPB_BYTES_PER_SEC, %bx
	jmp goOnLoadingFile

fileLoaded:
	# jmp fileLoaded

# set selector for protected code
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

# set selector for extra data
# lea extraData, %eax
# lea gdt, %ebx
# add $0x28, %ebx
# add $2, %ebx
# movw %ax, (%bx)
# shrl $16, %eax
# add $2, %bx
# movb %al, (%bx)
# add $3, %bx
# movb %ah, (%bx)

cli
inb $0x92, %al
orb $0x02, %al
outb %al, $0x92

lgdt gdtPtr

movl %cr0, %eax
orl $1, %eax
movl %eax, %cr0

xorl %eax, %eax
movw $SELECTOR_LEGACY_DATA_SEG, %ax
mov %ax, %ds
xorl %esi, %esi
mov $SELECTOR_STACK_SEG, %ax
add $RPL0, %ax
mov %ax, %ss
movl $TOP_OF_STACK, %eax
movl %eax, %esp
movl %eax, %ebp
ljmp $SELECTOR_CODE_SEG, $0

# read cl sectors into es:bx starting from sector ax
readSector:
	pushw %bp
	movw %sp, %bp
	subl $2, %esp
	movb %cl, -2(%bp)
	pushw %bx
	movb $BPB_SEC_PER_TRK, %bl
	divb %bl
	incb %ah
	movb %ah, %cl
	movb %al, %dh
	shrb $1, %al
	movb %al, %ch
	andb $1, %dh
	popw %bx
	movb $BS_DRV_NUM, %dl
	goOnReading:
	mov $2, %ah
	mov -2(%bp), %al
	int $0x13
	jc goOnReading
	addl $2, %esp
	popw %bp
	ret

getFATEntry:
	pushw %es
	pushw %bx
	pushw %ax
	movw $BASE_OF_KERNEL, %ax
	subw $0x100, %ax
	movw %ax, %es
	pop %ax
	movw $0, isOdd
	movw $3, %bx
	mulw %bx
	movw $2, %bx
	divw %bx
	cmpw $0, %dx
	jz isEven
	movw $1, isOdd
isEven:
	xor %dx, %dx
	movw $BPB_BYTES_PER_SEC, %bx
	divw %bx
	pushw %dx
	xorw %bx, %bx
	addw $SEC_NUM_OF_FAT1, %ax
	movb $2, %cl
	call readSector
	pop %dx
	add %dx, %bx
	movw %es:(%bx), %ax
	cmpw $1, isOdd
	jnz isEven2
	shrw $4, %ax
isEven2:
	andw $0xfff, %ax
getFATEntryOK:
	pop %bx
	pop %es
	ret

protectModeCode:
.code32
	xorl %eax, %eax
	movw kernelSize, %ax
	pushl %eax
	movl $LMA_BASE_OF_KERNEL, %eax
	pushl %eax
	call loadKernel
	popl %eax
	popl %eax

extraData:
kernelSize: .word 0
sectorNo: .word 0
rootDirSizeForLoop: .word ROOT_DIR_SECTORS
kernelFileName: .string "KERNEL  BIN"
isOdd: .word 0

gdt:
	GDT_DESCRIPTOR(0, 0, 0)
	GDT_DESCRIPTOR(0, 0xfff, GDT_CODE + GDT_32)		# code seg, set base addr in runtime
	GDT_DESCRIPTOR(LMA_BASE_OF_KERNEL, 0x2fff, GDT_DATA_RW)		# kernel file data seg
	GDT_DESCRIPTOR(0x800000, 0xffff, GDT_DATA_RW + DPL0)		# stack seg
	GDT_DESCRIPTOR(KERNEL_BASE_ADDRESS, 0xffff, GDT_CODE + GDT_32)	# flat code seg
	GDT_DESCRIPTOR(0x0, 0xffff, GDT_DATA_RW)	# legacy data seg
gdtPtr:
	.word 0x2f
	.long gdt
