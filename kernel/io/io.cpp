#include "io.h"

namespace os {

VideoOutStream videoOutStream;
IOStream& cout(videoOutStream);

IOStream::IOStream() : color(0x07) {}

VideoOutStream::VideoOutStream() : videoAddr((char*)0xb8000) {}
void VideoOutStream::out(const char* data) {
	char* video = videoAddr;
	while (*data != 0) {
		*video++ = *data++;
		*video++ = color;
	}
}

void operator << (IOStream& iostream, const char* data) {
	iostream.out(data);
}

}