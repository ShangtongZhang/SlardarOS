#include "defs.h"
#include "elf.h"
#include "bootLoader.h"

typedef struct{
	uint32_t offset;
	uint16_t selector;
} LjmpOperand;

void memoryCopy(uint8_t* dest, uint8_t* src, size_t size) {
	uint8_t* end = src + size;
	while (src < end) {
		*dest = *src;
		src++;
		dest++;
	}
}

void loadKernel(void* pKernel, size_t size) {
	Elf32Header* pElfHeader = (Elf32Header*)pKernel;
	Elf32ProgramHeader* pmHeaders = (Elf32ProgramHeader*)((uint32_t)pKernel + pElfHeader->ePHOff);
	uint16_t pHeaderEntSize = pElfHeader->ePHEntSize;
	uint16_t pmHeadersNum = pElfHeader->ePHNum;
	Elf32ProgramHeader* pPmHeader;
	for (uint16_t i = 0; i < pmHeadersNum; ++i) {
		pPmHeader = &pmHeaders[i];
		uint32_t pPAddr = pPmHeader->pPAddr;
		uint32_t pOffset = pPmHeader->pOffset;
		uint32_t pSegSize = pPmHeader->pFileSize;
		memoryCopy((uint8_t*)pPAddr, (uint8_t*)pKernel + pOffset, pSegSize);
	}
	LjmpOperand kernelMainAddr;
	kernelMainAddr.offset = pElfHeader->eEntry - KERNEL_BASE_ADDRESS;
	kernelMainAddr.selector = SELECTOR_KERNEL_CODE_SEG;
	__asm__("ljmp *%0"
			::"m"(kernelMainAddr));
}