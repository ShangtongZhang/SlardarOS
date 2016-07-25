#include "utils.h"
#include "mem/memoryManage.h"

extern "C" void __cxa_pure_virtual() { while (true); }

namespace std {
	void __throw_length_error(char const*) {}
	void __throw_logic_error(char const*) {}
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