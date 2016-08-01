#include "interruption.h"
#include "sys/io.h"
#include "utils/utils.h"
#include "clock.h"

uint64_t idt[IDT_SIZE];

void defaultIntrHandler() {
	while (true) {}
}

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
	for (size_t i = 0; i < IDT_SIZE; ++i) {
		idt[i] = GateDescriptor(defaultIntrHandler);
	}
}

void installISR() {
	idt[CLOCK_VECTOR] = GateDescriptor(clockIntrHandler);
}

void initInterruption() {
	init8259A();
	initClock();
	initIDT();
	installISR();

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