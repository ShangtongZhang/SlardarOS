#ifndef __MEMORY_MANAGER_HPP
#define __MEMORY_MANAGER_HPP
#include "defs.h"

namespace os {
namespace mem {

class MemoryManager {
public:
	virtual void* allocateMemory(size_t size) = 0;
	virtual void* allocateMemory(size_t size, uint32_t alignment) = 0;
	virtual void freeMemory(void* ptr) = 0;
	virtual void addMemory(void* addr, size_t length) = 0;
	virtual void reclaim() = 0;

	/**
	 * Max memory address is used in paging.
	 * For simplicity, paging uses [0, maxAddress)
	 */ 
	virtual uint32_t maxAddress() = 0;

	virtual ~MemoryManager() {};
};

} // mem
} // os

#endif