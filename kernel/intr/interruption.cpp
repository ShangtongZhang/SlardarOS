#include "interruption.h"
#include "sys/io.h"
#include "functional"

uint64_t idt[IDT_SIZE];

void defaultIntrHandler() {
	while (true) {}
}

void init8259A() {

	// Initialization Command Word 1
	outb(0x11, M8259A_PORT1);
	outb(0x11, S8259A_PORT1);

	// ICW2
	outb(IRQ0_VECTOR, M8259A_PORT2);
	outb(IRQ8_VECTOR, S8259A_PORT2);

	// ICW3
	outb(0x4, M8259A_PORT2);
	outb(0x2, S8259A_PORT2);

	// ICW4
	outb(0x1, M8259A_PORT2);
	outb(0x1, S8259A_PORT2);

	// Operation Control Word
	outb(~OCW_CLOCK, M8259A_PORT2);
	outb(0xff, S8259A_PORT2);

}

void initIDT() {
	for (size_t i = 0; i < IDT_SIZE; ++i) {
		idt[i] = GateDescriptor(static_cast<uint16_t>(GDT_SELECTOR_CODE), 
				reinterpret_cast<uint32_t>(defaultIntrHandler),
				static_cast<uint8_t>(GateDescriptor::GateAttr::interruptionGate));
	}
}

void initInterruption() {
	init8259A();
	initIDT();

	class __attribute__((__packed__)) {
	public:
		uint16_t length;
		uint32_t addr;
	} op;

	op.length = IDT_SIZE * sizeof(uint64_t) - 1;
	op.addr = reinterpret_cast<uint32_t>(idt);
	__asm__("lidt (%0)"
			::"p"(&op));
}