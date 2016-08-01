#ifndef __INTERRUPTION_H
#define __INTERRUPTION_H
#include "defs.h"

class GateDescriptor {
private:
	uint16_t selector;
	uint32_t offset;
	uint8_t attr;
	uint8_t paramCount;
public:
	GateDescriptor(uint16_t selector_, uint32_t offset_, uint8_t attr_, uint8_t paramCount_ = 0) :
		selector(selector_), offset(offset_), attr(attr_), paramCount(paramCount_) {

	}

	operator uint64_t () {
		class __attribute__((__packed__)) {
		public:
			uint16_t offsetLow;
			uint16_t selector;
			uint16_t attr;
			uint16_t offsetHigh;
		} gate;
		gate.offsetLow = static_cast<uint16_t>(offset & 0xffff);
		gate.offsetHigh = static_cast<uint16_t>((offset >> 16) & 0xffff);
		gate.selector = selector;
		gate.attr = static_cast<uint16_t>(attr) << 8;
		gate.attr |= paramCount & 0x1f;
		return *reinterpret_cast<uint64_t*>(&gate);
	}

	enum GateAttr {
		interruptionGate = 0x8e,
		trapGate = 0x8f,
		callGate = 0x8c
	};
};

void initInterruption();

constexpr size_t IDT_SIZE = 256;
constexpr size_t GDT_SELECTOR_CODE = 0x20;
constexpr uint16_t M8259A_PORT1 = 0x20;
constexpr uint16_t M8259A_PORT2 = 0x21;
constexpr uint16_t S8259A_PORT1 = 0xa0;
constexpr uint16_t S8259A_PORT2 = 0xa1;
constexpr uint8_t IRQ0_VECTOR = 0x20;
constexpr uint8_t IRQ8_VECTOR = 0x28;
constexpr uint8_t CLOCK_VECTOR = IRQ0_VECTOR;
constexpr uint8_t OCW_CLOCK = 0x1;
extern uint64_t idt[IDT_SIZE];

#endif