#include "utils/utils.h"
#include "utils/stl.hpp"
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

	os::string s1("Hello");
	os::string s2("World!");
	os::string s3 = s1 + " " + s2;
	size_t cnt = s3.size();
	os::vector<int> v{2, 3, 4};
	int sum = 0;
	for (auto it = v.begin(); it < v.end(); ++it) {
		sum += *it;
	}
	while (true) {}
	return 0;
}