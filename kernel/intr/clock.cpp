#include "interruption.h"
#include "sys/io.h"
#include "utils/utils.h"
#include "clock.h"

void clockHandler() {
	os::utils::clock++;
	os::io::cout << os::io::cls << os::utils::clock;
	outb(END_OF_INTR, MASTER_8259A_PORT);
}

void initPIT() {
	intrHandlers[Interruption::clock] = clockHandler;
	outb(PIT_PORT_MODE, PIT_SELECTOR_0 | PIT_RATE_GEN | PIT_16BIT);
	uint8_t low8Bit = (PIT_FREQ / PIT_DEMANDED_INTERVAL) & 0x00ff;
	uint8_t high8Bit = static_cast<uint8_t>(((PIT_FREQ / PIT_DEMANDED_INTERVAL) & 0xff00) >> 8);
	outb(PIT_PORT_CHANNEL0, low8Bit);
	outb(PIT_PORT_CHANNEL0, high8Bit);
}

void initClock() {
	initPIT();
}