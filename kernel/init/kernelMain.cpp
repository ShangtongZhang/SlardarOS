#include "memoryManage/memoryManage.h"

extern "C" int kernelMain() {
	initMemory();
	while (true) {}
	return 0;
}