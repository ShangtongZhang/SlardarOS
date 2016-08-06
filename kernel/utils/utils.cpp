#include "utils.h"
#include "mem/memoryManage.h"

extern "C" void __cxa_pure_virtual() { while (true) {} }

void* __dso_handle;
extern "C" int __cxa_atexit(void (*) (void *), void*, void*) { return 0; }

namespace std {
	void __throw_length_error(char const*) { while (true) {} }
	void __throw_logic_error(char const*) { while (true) {} }
	void __throw_bad_function_call() { while (true) {} }
}

namespace os {
namespace utils {

Clock clock;

}
}