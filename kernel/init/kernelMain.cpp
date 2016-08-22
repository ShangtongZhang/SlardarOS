#include "initialization.h"
#include "mem/memory.h"
#include "mem/virtualMemory.h"
#include "driver/clock.h"
#include "intr/interruption.h"

#include "test/test.hpp"

extern "C" int kernelMain() {
	os::init::initGDT();
	os::init::initBSS();
	os::mem::initMem();

	performUnitTests();

	os::clock::initClock();
	os::intr::initIntr();
	os::mem::vm::initVM();

	os::io::cout << os::io::cls << "Welcome to Slardar OS\n";
	while (true) {}
	return 0;
}