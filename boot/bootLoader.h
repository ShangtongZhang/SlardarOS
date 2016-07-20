#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#define MEMORY_INFO_UNIT_SIZE 20
#define BASE_OF_MEMORY_INFO 0x300
#define LMA_BASE_OF_MEMEORY_INFO 0x3000

#define BASE_OF_KERNEL 0x400
#define LMA_BASE_OF_KERNEL 0x4000
#define OFFSET_OF_KERNEL 0
#define KERNEL_BASE_ADDRESS 0x100000

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
	.word type & 0xffff;\
	.byte (base >> 24) & 0xff

#define GDT_CODE 0x98
#define GDT_32 0x4000
#define GDT_DATA_RW 0x92

#define SELECTOR_CODE_SEG 0x8
#define SELECTOR_KERNEL_FILE_DATA_SEG 0x10
#define SELECTOR_STACK_SEG 0x18
#define SELECTOR_KERNEL_CODE_SEG 0x20
#define SELECTOR_LEGACY_DATA_SEG 0x28

#define TOP_OF_STACK 0xfff

#define DPL0 0x00
#define DPL1 0x20
#define DPL2 0x40
#define DPL3 0x60

#define RPL0 0
#define RPL1 1
#define RPL2 2
#define RPL3 3

#endif