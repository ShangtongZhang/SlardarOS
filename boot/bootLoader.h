#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

/**
 * Memory Map:
 * 0x0600 - 0x6ff 	real mode stack
 * 0x0700 - 0xcff 	loader.bin
 * 0x0d00 - 0xfff 	memory info area
 * 0x1000 - 0x1fff 	fat entry area
 * 0x2000 -			kernel.bin
 */

#define MEMORY_INFO_UNIT_SIZE 20
#define BASE_OF_MEMORY_INFO 0xd0
#define LMA_BASE_OF_MEMEORY_INFO 0xd00

// Pay attention to getFATEntry when modify BASE_OF_KERNEL
#define BASE_OF_KERNEL 0
#define LMA_BASE_OF_KERNEL 0x2000
#define OFFSET_OF_KERNEL 0x2000
#define KERNEL_BASE_ADDRESS 0x100000

#define BASE_OF_FAT_ENTRY_AREA 0x100

#define BPB_SEC_PER_TRK 18
#define BS_DRV_NUM 0
#define ROOT_DIR_SECTORS 14
#define DELTA_SECTOR_NUM 17
#define BPB_BYTES_PER_SEC 512
#define SEC_NUM_OF_FAT1 1

#define GDT_DESCRIPTOR(base, limit, type)\
	.word limit & 0xffff;\
	.word base & 0xffff;\
	.byte (base >> 16) & 0xff;\
	.word (type & 0xf0ff) | ((limit & 0x0f0000) >> 8);\
	.byte (base >> 24) & 0xff

#define GDT_CODE 0x98
#define GDT_32 0x4000
#define GDT_4K 0x8000
#define GDT_DATA_RW 0x92

#define SELECTOR_CODE_SEG 0x8
#define SELECTOR_FLAT_STACK_SEG 0x10
#define SELECTOR_FLAT_DATA_SEG 0x18
#define SELECTOR_FLAT_CODE_SEG 0x20

#define TOP_OF_STACK 0x1fffff
#define BASE_OF_HEAP 0x200000
#define END_OF_KENEL_SPACE 0x2000000

#define DPL0 0x00
#define DPL1 0x20
#define DPL2 0x40
#define DPL3 0x60

#define RPL0 0
#define RPL1 1
#define RPL2 2
#define RPL3 3

#endif