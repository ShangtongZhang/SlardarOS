#ifndef __INTERRUPTION_H
#define __INTERRUPTION_H
#include "defs.h"

#define ISR_RETURN __asm__("leave;iret")

class GateDescriptor {
private:
	uint16_t selector;
	uint32_t offset;
	uint8_t attr;
	uint8_t paramCount;
public:
	template <typename T>
	GateDescriptor(T* offset_, 
		uint16_t selector_ = static_cast<uint16_t>(SELECTOR_FLAT_CODE_SEG),
		uint8_t attr_ = static_cast<uint8_t>(GateAttr::interruptionGate), 
		uint8_t paramCount_ = 0) :
		selector(selector_), 
		offset(reinterpret_cast<uint32_t>(offset_)), 
		attr(attr_), paramCount(paramCount_) {

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
constexpr uint16_t MASTER_8259A_PORT = 0x20;
constexpr uint16_t SlAVE_8259A_PORT = 0xa0;

constexpr uint8_t IRQ0_VECTOR = 0x20;
constexpr uint8_t IRQ8_VECTOR = 0x28;
constexpr uint8_t CLOCK_VECTOR = IRQ0_VECTOR;

constexpr uint8_t OCW_CLOCK = 0x1;

constexpr uint8_t END_OF_INTR = 0x20;

extern uint64_t idt[IDT_SIZE];

#endif