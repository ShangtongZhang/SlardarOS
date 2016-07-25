#include "utils/utils.h"
#include "mem/memoryManage.h"
#include "test/test.hpp"

void initBssVariables() {
	new (&os::mem::memoryManager) os::mem::PlainMemoryManager{};
	new (&os::cout) os::VideoOutStream{};
}

extern "C" int kernelMain() {
	initBssVariables();
	performUnitTests();

	initMemory();

	while (true) {}
	return 0;
}