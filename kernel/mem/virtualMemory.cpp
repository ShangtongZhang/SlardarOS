#include "virtualMemory.h"
#include "memory.h"
#include "intr/interruption.h"
#include "plainPagePool.hpp"

namespace os {
namespace mem {
namespace vm {

namespace hidden {

void pageFaultHandler(uint32_t errorCode) {
	os::mem::vm::vmController.pageFaultHandler(errorCode);
}

} // hidden

PlainVMController plainVMController;
VMController& vmController(plainVMController);

void initVM() {
	vmController.initPagePool<PlainPagePool>(10);
	os::intr::intrHandlers[os::intr::Interruption::pageFault] = hidden::pageFaultHandler;
	uint32_t kernelPageDir = vmController.createKernelPageDir();
	vmController.setKernelPageDir(kernelPageDir);
	vmController.applyPageDir(kernelPageDir);
	vmController.enablePaging();

	uint32_t userPageDir = vmController.createUserPageDir();
	vmController.applyPageDir(userPageDir);

	__asm__("movl 0x20000005, %%eax"
			:::"%eax");
}

} // vm
} // mem
} // os