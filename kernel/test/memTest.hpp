#ifndef __MEM_TEST_HPP
#define __MEM_TEST_HPP
#include "defs.h"
#include "assert.hpp"
#include "mem/plainMemoryManager.hpp"

void plainMemoryManagerTestBasic() {
	using namespace os::mem;
	constexpr size_t memSize = 1024;
	uint8_t mem[memSize];
	PlainMemoryManager pmm;
	MemoryManager& mm(pmm);
	mm.addMemory(static_cast<void*>(mem), memSize);

	size_t s1 = 10;
	size_t s2 = 20;
	size_t s3 = 30;
	uint8_t* p1 = static_cast<uint8_t*>(mm.allocateMemory(s1));
	uint8_t* p2 = static_cast<uint8_t*>(mm.allocateMemory(s2));
	uint8_t* p3 = static_cast<uint8_t*>(mm.allocateMemory(s3));
	size_t pmmSize = sizeof(PlainMemoryManager);

	assert(1 == 0);
	assert(true);
	assert(p1 == mem + pmmSize);
	assert(p2 == p1 + pmmSize);
	assert(p3 == p2 + pmmSize);
}

void plainMemoryManagerTest() {
	plainMemoryManagerTestBasic();
}

#endif