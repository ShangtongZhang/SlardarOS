#ifndef __VIRTUAL_MEMORY_H
#define __VIRTUAL_MEMORY_H
#include "defs.h"
#include "plainVMController.hpp"

namespace os {
namespace mem {
namespace vm {

void initVM();

extern PlainVMController plainVMController;
extern VMController& vmController;

} // vm
} // mem
} // os

#endif