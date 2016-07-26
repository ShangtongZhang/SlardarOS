#include "iostream.hpp"

namespace os {
namespace io {

template <>
IOStream& operator << (IOStream& iostream, const os::string& data) {
	return iostream.output(data);
}

template <>
IOStream& operator << (IOStream& iostream, const EndLine&) {
	return iostream.endLine();
}

VideoOutStream& operator << (VideoOutStream& videoOutStream, const ClearScreen&) {
	return videoOutStream.clearScreen();
}

}
}