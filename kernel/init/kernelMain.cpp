#include "utils/utils.h"
#include "utils/stl.hpp"
#include "mem/memoryManage.h"
#include "mem/virtualMemory.h"
#include "test/test.hpp"
#include "new"
#include "intr/interruption.h"

void initBssVariables() {
	for (size_t i = 0; i < IDT_SIZE; ++i) {
		new (intrHandlers + i) std::function<void()>{};
	}
	new (&os::mem::memoryManager) os::mem::PlainMemoryManager{};
	new (&os::io::cout) os::io::VideoOutStream{};
	new (&os::utils::clock) os::utils::Clock{};
}

extern "C" int kernelMain() {

	initBssVariables();
	performUnitTests();

	initMemory();
	initInterruption();
	initVirtualMemory();
	
	os::io::cout << os::io::cls << "Welcome to Slardar OS\n";
	while (true) {}
	return 0;
}