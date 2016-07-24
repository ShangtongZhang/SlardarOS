#include "io/io.h"
#include "mem/memoryManage.h"
#include "utils/utils.h"
#include "new"
#include "test/assert.hpp"

void initBssVariables() {
	new (&os::cout) os::VideoOutStream;
}

extern "C" int kernelMain() {
	initBssVariables();
	assert(1 == 3);
	initMemory();
	while (true) {}
	return 0;
}