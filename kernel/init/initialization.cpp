#include "initialization.h"
#include "utils/utils.h"
#include "utils/stl.hpp"
#include "mem/memory.h"
#include "mem/virtualMemory.h"
#include "driver/clock.h"
#include "intr/interruption.h"
#include "new"

namespace os {
namespace init {

constexpr size_t GDT_SIZE = 5;
uint64_t gdt[GDT_SIZE];

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
	using GDTAttr = GDTDescriptor::GDTAttr;
	gdt[0] = GDTDescriptor(0, 0, 0);
	gdt[1] = GDTDescriptor(0, 0xffffff, 
		GDTAttr::code | GDTAttr::bit32 | GDTAttr::granularity4K | GDTAttr::DPL0);
	gdt[2] = GDTDescriptor(0, 0xffffff,
		GDTAttr::dataRW | GDTAttr::bit32 | GDTAttr::granularity4K | GDTAttr::DPL0);
	gdt[3] = GDTDescriptor(0, 0xffffff,
		GDTAttr::code | GDTAttr::bit32 | GDTAttr::granularity4K | GDTAttr::DPL3);
	gdt[4] = GDTDescriptor(0, 0xffffff,
		GDTAttr::dataRW | GDTAttr::bit32 | GDTAttr::granularity4K | GDTAttr::DPL3);

	class __attribute__((__packed__)) {
	public:
		uint16_t length;
		uint32_t addr;
	} op;
	op.length = GDT_SIZE * sizeof(uint64_t) - 1;
	op.addr = reinterpret_cast<uint32_t>(gdt);
	__asm__("lgdt (%0);"
			::"p"(&op));
	__asm__("movw %%ax, %%ds;"
			::"a"(GDTSelector::dataKernel));
	__asm__("movw %%ax, %%ss;"
			::"a"(GDTSelector::dataKernel));
	__asm__("movw %%ax, %%es;"
			::"a"(GDTSelector::dataKernel));
	__asm__("movw %%ax, %%fs;"
			::"a"(GDTSelector::dataKernel));
	__asm__("movw %%ax, %%gs;"
			::"a"(GDTSelector::dataKernel));
	__asm__("ljmp %0, $fake;"
			"fake:;"
			::"i"(GDTSelector::codeKernel));
}

} // init
} // os