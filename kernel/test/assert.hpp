#ifndef __ASSERT_HPP
#define __ASSERT_HPP
#include "io/io.h"

#ifdef NO_DEBUG
#define assert(EX)
#else
#define assert(EX) (void)((EX) || (__assert (#EX, __FILE__, __LINE__),0))
#endif

void __assert (const char *msg, const char *file, int line) {
	os::cout << msg;
}

#endif