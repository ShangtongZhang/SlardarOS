#ifndef __UTILS_H
#define __UTILS_H
#include "defs.h"
#include "io/io.h"
#include "test/assert.h"
#include "new"

namespace os {
namespace utils {

void setSegmentRegisters(uint16_t);

} // utils
} // os

#endif