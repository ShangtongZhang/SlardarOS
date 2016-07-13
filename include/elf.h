#ifndef __ELF_H
#define __ELF_H

#include "defs.h"
#define EI_NIDENT 16

typedef uint32_t Elf32Addr;
typedef uint16_t Elf32Half;
typedef uint32_t Elf32Off;
typedef int32_t Elf32SWord;
typedef uint32_t Elf32Word;

typedef struct{
	unsigned char eIdent[EI_NIDENT];
	Elf32Half eType;
	Elf32Half eMachine;
	Elf32Word eVersion;
	Elf32Addr eEntry;
	Elf32Off ePHOff;
	Elf32Off eSHOff;
	Elf32Word eFlags;
	Elf32Half eCHSize;
	Elf32Half ePHEntSize;
	Elf32Half ePHNum;
	Elf32Half eSHEntSize;
	Elf32Half eSHNum;
	Elf32Half eSHStrPos;
} Elf32Header;

typedef struct{
	Elf32Word pType;
	Elf32Off pOffset;
	Elf32Addr pVAddr;
	Elf32Addr pPAddr;
	Elf32Word pFileSize;
	Elf32Word pMemSize;
	Elf32Word pFlags;
	Elf32Word pAlign;
} Elf32ProgramHeader;


#endif