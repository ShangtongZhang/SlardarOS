#include "utils.h"
#include "mem/memoryManage.h"

extern "C" void __cxa_pure_virtual() { while (true) {} }

namespace std {
	void __throw_length_error(char const*) { while (true) {} }
	void __throw_logic_error(char const*) { while (true) {} }
}

namespace os {
namespace utils {

Clock clock;

}
}