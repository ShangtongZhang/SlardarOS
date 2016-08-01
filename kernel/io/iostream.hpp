#ifndef __IOSTREAM_HPP
#define __IOSTREAM_HPP
#include "defs.h"
#include "utils/stl.hpp"

namespace os {
namespace io {

class EndLine {};

class IOStream {
public:
	virtual IOStream& output(const os::string&) = 0;
	virtual IOStream& endLine() = 0;
};

class ClearScreen {};

class VideoOutStream : public IOStream {
protected:
	char* const videoAddr;
	uint8_t color;
	size_t pos;
	size_t line;
	size_t column;
public:

	static const uint32_t VIDEO_ADDR = 0xb8000;
	static const size_t VIDEO_WIDTH = 80 * 2;
	static const size_t VIDEO_HEIGHT = 25;
	static const size_t SCREEN_SIZE = VIDEO_WIDTH * VIDEO_HEIGHT;
	enum DisplayColor {
		BLACK_WHITE = 0x07
	};

	VideoOutStream() : 
		videoAddr(reinterpret_cast<char*>(VIDEO_ADDR)),
		color(DisplayColor::BLACK_WHITE),
		pos(0), line(0), column(0) {
	}

	virtual IOStream& output(const os::string& str) override {
		char* video = videoAddr;
		for (auto it = str.begin(); it < str.end(); ++it) {
			line = pos / VIDEO_WIDTH;
			column = pos % VIDEO_WIDTH;
			if (*it == '\n') {
				// \n equals \n\r in SlardarOS
				line++;
				column = 0;
				pos = line * VIDEO_WIDTH % SCREEN_SIZE;
				continue;
			}
			video[pos++] = *it;
			video[pos++] = color;
			pos %= SCREEN_SIZE;
		}
		return *this;
	}

	virtual IOStream& endLine() override {
		return output("\n");
	}

	VideoOutStream& clearScreen() {
		char* video = videoAddr;
		char* end = video + VIDEO_WIDTH * VIDEO_HEIGHT;
		while (video < end) {
			*video++ = 0;
		}
		line = column = pos = 0;
		return *this;
	}
};

template <typename T>
IOStream& operator << (IOStream& iostream, const T& data) {
	return iostream.output(os::to_string(data));
}

template <>
IOStream& operator << (IOStream& iostream, const os::string& data);

template <>
IOStream& operator << (IOStream& iostream, const EndLine&);

VideoOutStream& operator << (VideoOutStream& vos, const ClearScreen&);

} // io
} // os

#endif