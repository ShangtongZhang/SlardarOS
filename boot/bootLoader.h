#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#define GDT_DESCRIPTOR(base, limit, type)\
	.word limit & 0xffff;\
	.word base & 0xffff;\
	.byte (base >> 16) & 0xff;\
	.word type & 0xffff;\
	.byte (base >> 24) & 0xff

#define GDT_CODE 0x98
#define GDT_32 0x4000

#endif