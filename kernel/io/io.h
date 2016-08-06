#ifndef __IO_H
#define __IO_H
#include "defs.h"
#include "iostream.hpp"

namespace os {
namespace io {

extern VideoOutStream& cout;
extern ClearScreen cls;
extern EndLine endl;

} // io
} // os

#endif