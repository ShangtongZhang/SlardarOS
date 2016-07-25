#include "io/io.h"
#include "memoryManage.h"
#include "plainMemoryManager.hpp"

namespace os {
namespace mem {
	PlainMemoryManager plainMemoryManager;
	MemoryManager& memoryManager(plainMemoryManager);
}
}


void initMemory() {
	uint32_t memoryInfoAddr = LMA_BASE_OF_MEMEORY_INFO;
	uint32_t baseOfHeap = BASE_OF_HEAP;
	uint16_t* memInfoUnitNum = (uint16_t*)memoryInfoAddr;
	os::mem::MemoryInfoUnit* memoryInfoUnits = 
		reinterpret_cast<os::mem::MemoryInfoUnit*>((memoryInfoAddr + sizeof(uint16_t)));
	for (uint16_t i = 0; i < *memInfoUnitNum; ++i) {
		uint32_t startAddr = memoryInfoUnits[i].baseAddrLow;
		uint32_t length = memoryInfoUnits[i].lengthLow;
		uint32_t flag = memoryInfoUnits[i].flag;
		if (flag == os::mem::MemoryInfoUnit::MemoryType::available) {
			uint32_t endAddr = startAddr + length;
			if (endAddr > baseOfHeap) {
				if (startAddr < baseOfHeap) {
					startAddr = baseOfHeap;
					os::mem::memoryManager.addMemory(
						reinterpret_cast<void*>(startAddr),
						endAddr - startAddr + 1);
				}
			}
		}
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