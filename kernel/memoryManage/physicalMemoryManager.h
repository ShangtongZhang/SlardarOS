#ifndef __PHYSICAL_MEMORY_MANAGER_H
#define __PHYSICAL_MEMORY_MANAGER_H
#include "defs.h"

namespace os {

class MemoryInfoUnit {
public:
	uint32_t baseAddrLow;
	uint32_t baseAddrHigh;
	uint32_t lengthLow;
	uint32_t lengthHigh;
	uint32_t flag;
	static uint32_t flagAvailable;
};

class RawMemoryChunk {
public:
	uint32_t beginAddr;
	uint32_t endAddr;
	size_t size;
	RawMemoryChunk* last;
	RawMemoryChunk* next;
	RawMemoryChunk();
};

class PhysicalMemoryManager {
public:
	PhysicalMemoryManager(MemoryInfoUnit* memoryInfoUnits, size_t memoryInfoUnitsCount);
};

}


#endif