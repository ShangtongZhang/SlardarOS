#ifndef __STL_HPP
#define __STL_HPP
#include "defs.h"
#include "mem/memoryManage.h"
#include "vector"
#include "string"

namespace os {

template <typename T>
using vector = std::vector<T, mem::PlainAllocator<T>>;

template <typename T>
using basic_string = std::basic_string<T, std::char_traits<T>, mem::PlainAllocator<T>>;

using string = basic_string<char>;

string to_string(unsigned int, size_t base = 10);
string to_string(int value, size_t base =  10);
string to_string(const char* data);
string to_string(char data);

}

#endif