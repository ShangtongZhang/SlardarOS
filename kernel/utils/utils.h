#ifndef __UTILS_H
#define __UTILS_H
#include "defs.h"

template <typename T1, typename T2>
void memoryCopy(T1* dest_, T2* src_, size_t size) {
	uint8_t* dest = reinterpret_cast<uint8_t*>(dest_);
	uint8_t* src = reinterpret_cast<uint8_t*>(src_);
	uint8_t* end = src + size;
	while (src < end) {
		*dest = *src;
		src++;
		dest++;
	}
}

#endif