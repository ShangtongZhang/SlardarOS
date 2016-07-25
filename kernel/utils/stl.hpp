#ifndef __STL_HPP
#define __STL_HPP
#include "utils.h"
#include "mem/memoryManage.h"
#include "vector"
// #include "string"

namespace os {
namespace mem {

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

}

template <typename T>
using vector = std::vector<T, mem::PlainAllocator<T>>;

// template <typename T>
// using basic_string = std::basic_string<T, std::char_traits<T>, mem::PlainAllocator<T>>;

// using string = basic_string<char>;

}

#endif