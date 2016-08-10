#include "virtualMemory.h"
#include "memory.h"
#include "intr/interruption.h"

namespace os {
namespace mem {
namespace vm {

namespace hidden {

void pageFaultHandler(uint32_t) {
	using os::mem::vm::vmController;
	uint32_t virtualAddr = vmController.getVirtualAddr();
	uint32_t kernelPageDir = vmController.getKernelPageDir();
	uint32_t oldPageDir = vmController.retrivePageDir();
	vmController.applyPageDir(kernelPageDir);
	uint32_t* pageDir = reinterpret_cast<uint32_t*>(oldPageDir);
	uint32_t PDEIndex = vmController.getPDEIndex(virtualAddr);
	vmController.PDEHandler(pageDir, PDEIndex);
	uint32_t PDE = pageDir[PDEIndex];
	uint32_t* pageTable = reinterpret_cast<uint32_t*>(vmController.getAddress(PDE));
	uint32_t PTEIndex = vmController.getPTEIndex(virtualAddr);
	vmController.PTEHandler(pageTable, PTEIndex);
	vmController.applyPageDir(oldPageDir);
}

} // hidden

PlainVMController plainVMController;
VMController& vmController(plainVMController);

void initVM() {
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