#ifndef __MEMORY_MANAGER_HPP
#define __MEMORY_MANAGER_HPP
#include "defs.h"

namespace os {
namespace mem {

class MemoryManager {
public:
	virtual void* allocateMemory(size_t size) = 0;
	virtual void freeMemory(void* ptr) = 0;
	virtual void addMemory(void* addr, size_t length) = 0;
	virtual void reclaim() = 0;
	
	/**
	 * Do not declare dtor explicitly, as ths os 
	 * doesn't have memory management functionality for now.
	 */
	//virtual ~MemoryManager();
};

}
}

#endif