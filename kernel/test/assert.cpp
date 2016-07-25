#include "assert.h"

void __assert (const char *msg, const char *file, int line) {
	os::cout << msg;
}