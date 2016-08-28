#include "ring.h"

namespace os {
namespace intr {

void kernelProxy(SystemCallType) {
	size_t systemCall = 0;
	__asm__("movl 12(%%ebp), %%eax"
			:"=a"(systemCall));
	os::utils::setSegmentRegisters(GDTSelector::dataKernel + GDTSelector::RPL0);
	__asm__("leave;"
			"retf $4");
}

void systemCall(SystemCallType systemCall) {
	class __attribute__((__packed__)) {
	public:
		uint32_t offset;
		uint16_t selector;
	} op;
	op.offset = 0;
	op.selector = GDTSelector::systemCall + GDTSelector::RPL3;
	__asm__("pushl %%eax"
			::"a"(systemCall));
	__asm__("lcall *%0"
			::"m"(op));
	__asm__("subl $4, %%esp"
			:::"%esp");
	os::utils::setSegmentRegisters(GDTSelector::dataUser + GDTSelector::RPL3);
}

} // intr
} // os