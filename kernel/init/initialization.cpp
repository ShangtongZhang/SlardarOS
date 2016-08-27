#include "initialization.h"
#include "utils/utils.h"
#include "utils/stl.hpp"
#include "mem/memory.h"
#include "mem/virtualMemory.h"
#include "driver/clock.h"
#include "intr/interruption.h"
#include "intr/ring.h"
#include "new"

namespace os {
namespace init {

namespace hidden {

}

constexpr size_t GDT_SIZE = 7;
uint64_t gdt[GDT_SIZE];
TaskStateSegment tss;

void enterRing0() {
	int n = 0;
	++n;
}

void initBSS() {
	for (size_t i = 0; i < os::intr::IDT_SIZE; ++i) {
		new (os::intr::intrHandlers + i) std::function<void(uint32_t)>{};
	}
	new (&os::mem::memoryManager) os::mem::PlainMemoryManager{};
	new (&os::io::cout) os::io::VideoOutStream{};
	new (&os::clock::clock) os::clock::Clock{};
	new (&os::mem::vm::vmController) os::mem::vm::PlainVMController{};
}

void initGDT() {
	using SegAttr = SegDescriptor::SegAttr;
	gdt[0] = SegDescriptor(0, 0, 0);
	gdt[1] = SegDescriptor(0, 0xffffff, 
		SegAttr::code | SegAttr::bit32 | SegAttr::granularity4K | SegAttr::DPL0);
	gdt[2] = SegDescriptor(0, 0xffffff,
		SegAttr::dataRW | SegAttr::bit32 | SegAttr::granularity4K | SegAttr::DPL0);
	gdt[3] = SegDescriptor(0, 0xffffff,
		SegAttr::code | SegAttr::bit32 | SegAttr::granularity4K | SegAttr::DPL3);
	gdt[4] = SegDescriptor(0, 0xffffff,
		SegAttr::dataRW | SegAttr::bit32 | SegAttr::granularity4K | SegAttr::DPL3);
	gdt[5] = SegDescriptor(reinterpret_cast<uint32_t>(&tss), sizeof(tss) - 1,
		SegAttr::tssSeg | SegAttr::DPL0);
	gdt[6] = GateDescriptor(os::intr::kernelProxy, GDTSelector::codeKernel + GDTSelector::RPL0,
		GateDescriptor::GateAttr::callGate + SegAttr::DPL3, 1);

	class __attribute__((__packed__)) {
	public:
		uint16_t length;
		uint32_t addr;
	} op;
	op.length = GDT_SIZE * sizeof(uint64_t) - 1;
	op.addr = reinterpret_cast<uint32_t>(gdt);
	__asm__("lgdt (%0);"
			::"p"(&op));
	__asm__("movw %%ax, %%ss;"
			::"a"(GDTSelector::dataKernel));
	os::utils::setSegmentRegisters(GDTSelector::dataKernel);
	__asm__("ljmp %0, $fake;"
			"fake:;"
			::"i"(GDTSelector::codeKernel));
}

void initTSS() {
	tss.ssRing0 = GDTSelector::dataKernel + GDTSelector::RPL0;
	tss.espRing0 = TOP_OF_KERNEL_STACK;
	__asm__("ltr %%ax"
			::"a"(GDTSelector::tssSeg));
}

void initialize() {
	initGDT();
	initTSS();
	initBSS();
}

} // init
} // os