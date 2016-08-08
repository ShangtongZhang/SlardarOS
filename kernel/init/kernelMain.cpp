#include "utils/utils.h"
#include "utils/stl.hpp"
#include "mem/memory.h"
#include "mem/virtualMemory.h"
#include "driver/clock.h"
#include "test/test.hpp"
#include "intr/interruption.h"
#include "new"

void initGlobalVariables() {
	for (size_t i = 0; i < os::intr::IDT_SIZE; ++i) {
		new (os::intr::intrHandlers + i) std::function<void(uint32_t)>{};
	}
	new (&os::mem::memoryManager) os::mem::PlainMemoryManager{};
	new (&os::io::cout) os::io::VideoOutStream{};
	new (&os::clock::clock) os::clock::Clock{};
}

extern "C" int kernelMain() {
	initGlobalVariables();
	os::mem::initMem();
	
	performUnitTests();

	os::clock::initClock();
	os::intr::initIntr();
	os::mem::vm::initVM();

	os::io::cout << os::io::cls << "Welcome to Slardar OS\n";
	while (true) {}
	return 0;
}