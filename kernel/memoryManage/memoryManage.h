#ifndef __MEMORY_MANAGE_H
#define __MEMORY_MANAGE_H
#include "defs.h"

class MemoryInfoUnit {
public:
	uint32_t baseAddrLow;
	uint32_t baseAddrHigh;
	uint32_t lengthLow;
	uint32_t lengthHigh;
	uint32_t flag;
	static uint32_t flagAvailable;
};

void initMemory();

#endif