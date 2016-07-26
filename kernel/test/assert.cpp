#include "assert.h"
#include "io/io.h"
#include "utils/stl.hpp"

void __assert (const char *msg, const char *file, int line) {
	os::string str("\nAssertion fail: ");
	str += msg;
	str += ", File: ";
	str += file;
	str += ", Line: ";
	str += os::to_string(line);
	str += "\n";
	os::io::cout << str;
	while (true) {}
}