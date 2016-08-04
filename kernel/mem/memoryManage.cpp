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
}

void* operator new (size_t count) {
	return os::mem::memoryManager.allocateMemory(count);
}

void* operator new[] (size_t count) {
	return os::mem::memoryManager.allocateMemory(count);
}

void operator delete (void* ptr) {
	os::mem::memoryManager.freeMemory(ptr);
}

void operator delete (void* ptr, size_t) {
	os::mem::memoryManager.freeMemory(ptr);
}

void operator delete[] (void* ptr) {
	os::mem::memoryManager.freeMemory(ptr);
}

void operator delete[] (void* ptr, size_t) {
	os::mem::memoryManager.freeMemory(ptr);
}