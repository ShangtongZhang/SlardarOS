#include "utils.h"
#include "mem/memory.h"

extern "C" void __cxa_pure_virtual() { while (true) {} }

void* __dso_handle;
extern "C" int __cxa_atexit(void (*) (void *), void*, void*) { return 0; }

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

namespace std {
	void __throw_length_error(char const*) { while (true) {} }
	void __throw_logic_error(char const*) { while (true) {} }
	void __throw_bad_function_call() { while (true) {} }
} // std

namespace os {
namespace utils {


} // utils
} // os