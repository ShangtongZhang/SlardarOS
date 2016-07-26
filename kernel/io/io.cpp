#include "io.h"

namespace os {
namespace io {

VideoOutStream videoOutStream;
VideoOutStream& cout(videoOutStream);

ClearScreen clearScreen;
ClearScreen& cls(clearScreen);

EndLine endLine;
EndLine& endl(endLine);

}
}