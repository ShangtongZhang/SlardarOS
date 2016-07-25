#ifndef __MEM_TEST_HPP
#define __MEM_TEST_HPP
#include "utils/utils.h"
#include "mem/plainMemoryManager.hpp"

void plainMemoryManagerTestBasic() {
	using namespace os::mem;
	constexpr size_t memSize = 1024;
	uint8_t mem[memSize];
	PlainMemoryManager pmm;
	MemoryManager& mm(pmm);
	mm.addMemory(static_cast<void*>(mem), memSize);

	// |s1|s2|s3|
	size_t s1 = 10;
	size_t s2 = 20;
	size_t s3 = 50;
	uint8_t* p1 = static_cast<uint8_t*>(mm.allocateMemory(s1));
	uint8_t* p2 = static_cast<uint8_t*>(mm.allocateMemory(s2));
	uint8_t* p3 = static_cast<uint8_t*>(mm.allocateMemory(s3));
	size_t MCS = sizeof(os::mem::hidden::MemoryChunk);

	assert(p1 == mem + MCS);
	assert(p2 == p1 + s1 + MCS);
	assert(p3 == p2 + s2 + MCS);

	mm.freeMemory(static_cast<void*>(p2));
	size_t s4 = 10;
	uint8_t* p4 = static_cast<uint8_t*>(mm.allocateMemory(s4));
	// |s1|s2(A)|s3|s4|

	assert(p4 == p3 + s3 + MCS);

	uint8_t* p3_ = p3;
	mm.freeMemory(static_cast<void*>(p3));
	size_t s5 = 10;
	uint8_t* p5 = static_cast<uint8_t*>(mm.allocateMemory(s5));
	// |s1|s2(A)|s5|s3 - s5 - MCS (A)|s4|

	assert(p5 == p3_);

	mm.freeMemory(static_cast<void*>(p5));
	mm.reclaim();
	// |s1|s2 + MCS + s5 + MCS + s3 - s5 - MCS (A)|s4|

	// s6 = s2 + s3 + MCS
	uint8_t s6 = s2 + MCS + s5 + MCS + s3 - s5 - MCS;
	uint8_t* p6 = static_cast<uint8_t*>(mm.allocateMemory(s6));
	// |s1|s2 + s3 + MCS (A)|s4|s6|

	assert(p6 == p4 + s4 + MCS);

	uint8_t s7 = s6 - MCS;
	uint8_t* p7 = static_cast<uint8_t*>(mm.allocateMemory(s7));
	// |s1|s7||s4|s6|

	assert(p7 == p1 + s1 + MCS);
}

void plainMemoryManagerTest() {
	plainMemoryManagerTestBasic();
}

#endif