#include "ring.h"

namespace os {
namespace intr {

void kernelProxy(size_t) {
	os::utils::setSegmentRegisters(GDTSelector::dataKernel + GDTSelector::RPL0);
}

void enterRing0(SystemCall systemCall) {
	class __attribute__((__packed__)) {
	public:
		uint32_t offset;
		uint16_t selector;
	} op;
	op.offset = 0;
	op.selector = GDTSelector::ring0 + GDTSelector::RPL3;
	__asm__("pushl %%eax"
			::"a"(systemCall));
	__asm__("lcall *%0"
			::"m"(op));
}

} // intr
} // os