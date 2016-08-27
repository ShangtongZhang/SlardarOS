#ifndef __INITIALIZATION_H
#define __INITIALIZATION_H
#include "defs.h"
#include "init/infrastructure.hpp"

namespace os {
namespace init {

extern TaskStateSegment tss;
void initialize();

} // init
} // os

#endif