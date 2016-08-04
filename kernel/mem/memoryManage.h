#ifndef __MEMORY_MANAGE_H
#define __MEMORY_MANAGE_H
#include "defs.h"
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

template <typename T1, typename T2>
void memoryCopy(T1* dest_, T2* src_, size_t size) {
	uint8_t* dest = reinterpret_cast<uint8_t*>(dest_);
	uint8_t* src = reinterpret_cast<uint8_t*>(src_);
	uint8_t* end = src + size;
	while (src < end) {
		*dest = *src;
		src++;
		dest++;
	}
}

template <typename T>
class PlainAllocator {
public:
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef void* void_pointer;
	typedef const void* const_void_pointer;
	typedef T value_type;
	typedef size_t size_type;
	typedef int32_t difference_type;

	PlainAllocator() {}
	PlainAllocator(const PlainAllocator&) {}
	template <typename U>
	PlainAllocator(const PlainAllocator<U>&) {}

	pointer allocate(size_type n) {
		return static_cast<pointer>(memoryManager.allocateMemory(n * sizeof(T)));
	}

	pointer allocate(size_type n, const_void_pointer) {
		return static_cast<pointer>(memoryManager.allocateMemory(n * sizeof(T)));
	}

	void deallocate(pointer p, size_type) {
		memoryManager.freeMemory(static_cast<void_pointer>(p));
	}
};

template <typename T>
bool operator == (const PlainAllocator<T>&, const PlainAllocator<T>&) {
	return true;
}

template <typename T>
bool operator != (const PlainAllocator<T>&, const PlainAllocator<T>&) {
	return false;
}

} // mem
} // os



const uint32_t PAGE_TABLE_SIZE = 0x400;

// static uint32_t PDEs[PAGE_DIR_SIZE];
// static uint32_t PTEs[PAGE_DIR_SIZE * PAGE_TABLE_SIZE];

void initMemory();

#endif