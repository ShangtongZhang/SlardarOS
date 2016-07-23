#include "io/io.h"
#include "memoryManage.h"
#include "plainMemoryManager.hpp"

uint32_t MemoryInfoUnit::flagAvailable = 1;

void initMemory() {
	os::cout << "Hello World!";
	uint32_t memoryInfoAddr = LMA_BASE_OF_MEMEORY_INFO;
	uint16_t* memInfoUnitNum = (uint16_t*)memoryInfoAddr;
	MemoryInfoUnit* memoryInfoUnits = (MemoryInfoUnit*)(memoryInfoAddr + 2);
	for (uint16_t i = 0; i < *memInfoUnitNum; ++i) {
		uint32_t baseAddrLow = memoryInfoUnits[i].baseAddrLow;
		uint32_t lengthLow = memoryInfoUnits[i].lengthLow;
		uint32_t flag = memoryInfoUnits[i].flag;
	}

	// uint32_t PDE = (uint32_t)PTEs | PAGE_P | PAGE_US_S | PAGE_RW_W;
	// for (int i = 0; i < PAGE_DIR_SIZE; ++i) {
	// 	PDEs[i] = PDE;
	// 	PDE += PAGE_TABLE_SIZE;
	// }
	// uint32_t PTE = 0 | PAGE_P | PAGE_US_S | PAGE_RW_W;
	// for (int i = 0; i < PAGE_DIR_SIZE * PAGE_TABLE_SIZE; ++i) {
	// 	PTEs[i] = PTE;
	// 	PTE += 0x1000;
	// }
	// uint32_t pageDirAddr = (uint32_t)PDEs;
	// __asm__("movl %0, %%eax;"
	// 		"movl %%eax, %%cr3;"
	// 		"movl %%cr0, %%eax;"
	// 		"orl $0x80000000, %%eax;"
	// 		"movl %%eax, %%cr0;"
	// 		::"r"(pageDirAddr)
	// 		:"%eax");
}