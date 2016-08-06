#include "interruption.h"
#include "sys/io.h"
#include "utils/utils.h"
#include "clock.h"
#include "mem/virtualMemory.h"
#include "array"

std::function<void()> intrHandlers[IDT_SIZE];

bool hasErrorCode(uint32_t intrNo) {
	switch (intrNo) {
		case 8:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 17:
		case 30:
			return true;
			break;
		default:
			return false;
			break;
	}
}

template <uint32_t intrNo>
void intrHandlerProxy() {
	__asm__("pushal");
	if (intrHandlers[intrNo]) {
		intrHandlers[intrNo]();
	}
	__asm__("popal");
	__asm__("leave");
	if (hasErrorCode(intrNo)) {
		__asm__("addl $4, %%esp":::"%esp");
	}
	__asm__("iret");
}

template <typename Is>
class intrHandlerHelper;

template <size_t ... Is>
class intrHandlerHelper<std::index_sequence<Is...>> {
public:
  static constexpr std::array<IntrHandler, sizeof...(Is)> makeIntrHandlers() {
    return {{ &intrHandlerProxy<Is> ... }};
  }
};

uint64_t idt[IDT_SIZE];

void init8259A() {
	// Initialization Command Word 1
	outb(0x11, MASTER_8259A_PORT);
	outb(0x11, SlAVE_8259A_PORT);

	// ICW2
	outb(IRQ0_VECTOR, MASTER_8259A_PORT + 1);
	outb(IRQ8_VECTOR, SlAVE_8259A_PORT + 1);

	// ICW3
	outb(0x4, MASTER_8259A_PORT + 1);
	outb(0x2, SlAVE_8259A_PORT + 1);

	// ICW4
	outb(0x1, MASTER_8259A_PORT + 1);
	outb(0x1, SlAVE_8259A_PORT + 1);

	// Operation Control Word
	outb(~OCW_CLOCK, MASTER_8259A_PORT + 1);
	outb(0xff, SlAVE_8259A_PORT + 1);
}

void initIDT() {
	constexpr std::array<IntrHandler, IDT_SIZE> intrHandlers = 
		intrHandlerHelper<std::make_index_sequence<IDT_SIZE>>::makeIntrHandlers();
	auto intrHandlersAddr = intrHandlers.data();
	for (size_t i = 0; i < IDT_SIZE; ++i) {
		idt[i] = GateDescriptor(intrHandlersAddr[i]);
	}
}

void initInterruption() {
	init8259A();
	initClock();
	initIDT();

	class __attribute__((__packed__)) {
	public:
		uint16_t length;
		uint32_t addr;
	} op;
	op.length = IDT_SIZE * sizeof(uint64_t) - 1;
	op.addr = reinterpret_cast<uint32_t>(idt);
	__asm__("lidt (%0);"
			"sti"
			::"p"(&op));
}