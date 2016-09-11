#ifndef __INTERRUPTION_H
#define __INTERRUPTION_H
#include "defs.h"
#include "functional"
#include "init/initialization.h"

namespace os {
namespace intr {

namespace hidden {

constexpr uint16_t MASTER_8259A_PORT = 0x20;
constexpr uint16_t SlAVE_8259A_PORT = 0xa0;
constexpr uint8_t IRQ0_VECTOR = 0x20;
constexpr uint8_t IRQ1_VECTOR = 0x21;
constexpr uint8_t IRQ8_VECTOR = 0x28;

constexpr uint8_t OCW_CLOCK = 0x1;
constexpr uint8_t OCW_KEYBOARD = 0x2;
constexpr uint8_t END_OF_INTR = 0x20;

} // hidden

void initIntr();
void endInterruption();

constexpr size_t IDT_SIZE = 256;

enum Interruption {
	clock = hidden::IRQ0_VECTOR,
	keyboard = hidden::IRQ1_VECTOR,
	pageFault = 0xe
};

extern std::function<void(uint32_t)> intrHandlers[IDT_SIZE];

} // intr
} // os

#endif