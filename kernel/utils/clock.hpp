#ifndef __CLOCK_HPP
#define __CLOCK_HPP
#include "defs.h"
#include "stl.hpp"

namespace os {
namespace utils {

class Clock {
private:
	uint32_t time;
public:
	Clock () : time(0) {

	}

	Clock& operator ++ (int) {
		time++;
		return *this;
	}

	uint32_t getTime () const {
		return time;
	}

	os::string toString () const {
		return os::to_string(time);
	}

};

} // utils
} // os

#endif