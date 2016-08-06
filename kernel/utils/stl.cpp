#include "stl.hpp"

namespace os {

string to_string_impl(int value, size_t base) {
	string dict("0123456789ABCDEF");
	vector<uint8_t> tmp;
	string result;
	if (value == 0) {
		result += '0';
		return result;
	}
	if (value < 0) {
		result += '-';
		value = -value;
	}
	while (value) {
		tmp.push_back(value % base);
		value /= base;
	}
	for (auto it = tmp.rbegin(); it < tmp.rend(); ++it) {
		result += dict[*it];
	}
	if (base == 16) {
		result += 'h';
	} else if (base == 2) {
		result += 'b';
	}
	return result;
}

string to_string(unsigned int value, size_t base) {
	return to_string_impl(value, base);
}

string to_string(int value, size_t base) {
	return to_string_impl(value, base);
}

string to_string(char data) {
	string str;
	str += data;
	return str;
}

string to_string(const char* data) {
	return data;
}

} // os