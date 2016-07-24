#include "io/io.h"
#include "mem/memoryManage.h"
#include "utils/utils.h"
#include "new"
#include "test/assert.hpp"
#include "test/test.hpp"

void initBssVariables() {
	new (&os::cout) os::VideoOutStream;
}

extern "C" int kernelMain() {
	initBssVariables();
	performUnitTests();

	initMemory();
	while (true) {}
	return 0;
}