#ifndef __MEMORY_MANAGE_H
#define __MEMORY_MANAGE_H
#include "utils/utils.h"
#include "plainMemoryManager.hpp"

namespace os {
namespace mem {

	// global memory manager
	extern MemoryManager& memoryManager;

	class MemoryInfoUnit {
	public:
		uint32_t baseAddrLow;
		uint32_t baseAddrHigh;
		uint32_t lengthLow;
		uint32_t lengthHigh;
		uint32_t flag;
		enum MemoryType {
			undefined = 0,
			available = 1,
			reserved = 2
		};
	};

}
}



const int PAGE_DIR_SIZE = 0x400;
const int PAGE_TABLE_SIZE = 0x400;

// static uint32_t PDEs[PAGE_DIR_SIZE];
// static uint32_t PTEs[PAGE_DIR_SIZE * PAGE_TABLE_SIZE];

const uint32_t PAGE_P = 1;
const uint32_t PAGE_RW_R = 0;
const uint32_t PAGE_RW_W = 2;
const uint32_t PAGE_US_S = 0;
const uint32_t PAGE_US_U = 4;

void initMemory();

#endif