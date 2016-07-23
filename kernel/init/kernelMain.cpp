#include "io/io.h"
#include "memoryManage/memoryManage.h"
#include "utils/utils.h"

void initBssVariable() {
	os::VideoOutStream videoOutStream;
	memoryCopy(&os::cout, &videoOutStream, sizeof(os::VideoOutStream));
}

extern "C" int kernelMain() {
	initBssVariable();
	initMemory();
	while (true) {}
	return 0;
}