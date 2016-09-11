#include "initialization.h"
#include "mem/memory.h"
#include "mem/virtualMemory.h"
#include "driver/clock.h"
#include "driver/keyboard.h"
#include "intr/interruption.h"
#include "intr/ring.h"

#include "test/test.hpp"

extern "C" int kernelMain() {
	os::init::initialize();
	// os::intr::enterRing3([](){});
	os::mem::initMem();

	performUnitTests();

	os::clock::initClock();
	os::keyboard::initKeyboard();
	os::intr::initIntr();
	os::mem::vm::initVM();

	os::io::cout << os::io::cls << "Welcome to Slardar OS\n";
	while (true) {}
	return 0;
}