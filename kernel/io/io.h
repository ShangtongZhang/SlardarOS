#ifndef __IO_H
#define __IO_H
#include "defs.h"

namespace os {

class IOStream {
protected:
	uint8_t color;
public:
	IOStream();
	friend void operator << (IOStream& iostream, const char* data);
	virtual void out(const char* data) = 0;
};

class VideoOutStream : public IOStream {
protected:
	char* videoAddr;
public:
	VideoOutStream();
	virtual void out(const char* data);
};

void operator << (IOStream& iostream, const char* data);

extern IOStream& cout;

}
#endif