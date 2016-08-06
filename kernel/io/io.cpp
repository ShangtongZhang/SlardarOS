#include "io.h"

namespace os {
namespace io {

VideoOutStream videoOutStream;
VideoOutStream& cout(videoOutStream);

ClearScreen cls;

EndLine endl;

} // io
} // os