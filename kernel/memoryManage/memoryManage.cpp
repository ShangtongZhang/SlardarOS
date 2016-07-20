#include "memoryManage.h"

uint32_t MemoryInfoUnit::flagAvailable = 1;

void initMemory() {
	uint32_t memoryInfoAddr = LMA_BASE_OF_MEMEORY_INFO;
	uint16_t* memInfoUnitNum = (uint16_t*)memoryInfoAddr;
	MemoryInfoUnit* memoryInfoUnits = (MemoryInfoUnit*)(memoryInfoAddr + 2);
	for (uint16_t i = 0; i < *memInfoUnitNum; ++i) {
		uint32_t baseAddrLow = memoryInfoUnits[i].baseAddrLow;
		uint32_t lengthLow = memoryInfoUnits[i].lengthLow;
		uint32_t flag = memoryInfoUnits[i].flag;
	}
}