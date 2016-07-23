#ifndef __MEMORY_MANAGER_HPP
#define __MEMORY_MANAGER_HPP
#include "defs.h"

namespace os {
namespace mem {

class MemoryManager {
public:
	virtual void* allocateMemory(size_t size) = 0;
	virtual void freeMemory(void* ptr) = 0;
	virtual ~MemoryManager();
};

}
}

#endif