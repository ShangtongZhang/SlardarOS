#include "utils/utils.h"
#include "utils/stl.hpp"
#include "mem/memoryManage.h"
#include "mem/virtualMemory.h"
#include "test/test.hpp"
#include "new"
#include "intr/interruption.h"

void initBssVariables() {
	new (&os::mem::memoryManager) os::mem::PlainMemoryManager{};
	new (&os::io::cout) os::io::VideoOutStream{};
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