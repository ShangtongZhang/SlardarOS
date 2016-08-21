#ifndef __STL_HPP
#define __STL_HPP
#include "defs.h"
#include "mem/memory.h"
#include "vector"
#include "string"
#include "map"

namespace os {

template <typename T>
using vector = std::vector<T, mem::PlainAllocator<T>>;

template <typename T>
using basic_string = std::basic_string<T, std::char_traits<T>, mem::PlainAllocator<T>>;

template<
    class Key,
    class T,
    class Compare = std::less<Key>
>
using map = std::map<Key, T, Compare, mem::PlainAllocator<std::pair<const Key, T>>>;

using string = basic_string<char>;

string to_string(unsigned int, size_t base = 10);
string to_string(int value, size_t base =  10);
string to_string(const char* data);
string to_string(char data);

template <typename T>
string to_string(const T& data) {
	return data.toString();
}

} // os

#endif