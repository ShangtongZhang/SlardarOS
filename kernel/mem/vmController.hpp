#ifndef __VM_CONTROLLER_HPP
#define __VM_CONTROLLER_HPP
#include "defs.h"

namespace os {
namespace mem {
namespace vm {

class VMController {
protected:
	uint32_t kernelPageDir;
public:
	static constexpr uint32_t ENTRY_PER_PAGE_DIR = 0x400;
	static constexpr uint32_t ENTRY_PER_PAGE_TABLE = 0x400;
	static constexpr uint32_t PAGE_SIZE = 0x1000;
	static constexpr uint32_t PAGE_NP = 0;
	static constexpr uint32_t PAGE_P = 1;
	static constexpr uint32_t PAGE_RW_R = 0;
	static constexpr uint32_t PAGE_RW_W = 2;
	static constexpr uint32_t PAGE_US_S = 0;
	static constexpr uint32_t PAGE_US_U = 4;

	// The page needs to be freed separately
	static constexpr uint32_t PAGE_TO_FREE = 0x200;
	static constexpr uint32_t PAGE_NOT_TO_FREE = 0;

	virtual uint32_t createKernelPageDir() = 0;
	virtual uint32_t createUserPageDir() = 0;
	virtual void PDEHandler(uint32_t* pageDir, uint32_t PDEIndex) = 0;
	virtual void PTEHandler(uint32_t* pageTable, uint32_t PTEIndex) = 0;
	uint32_t retrivePageDir() {
		uint32_t pageDir = 0;
		__asm__("movl %%cr3, %0;"
				:"=r"(pageDir));
		return pageDir;
	}
	void applyPageDir(uint32_t pageDir) {
		__asm__("movl %0, %%eax;"
				"movl %%eax, %%cr3;"
				::"r"(pageDir)
				:"%eax");
	}
	void enablePaging() {
		__asm__("movl %%cr0, %%eax;"
				"orl $0x80000000, %%eax;"
				"movl %%eax, %%cr0;"
				:::"%eax");
	}
	void setKernelPageDir(uint32_t pageDir) {
		kernelPageDir = pageDir;
	}
	uint32_t getKernelPageDir() {
		return kernelPageDir;
	}
	uint32_t getPDEIndex(uint32_t virtualAddr) {
		return (virtualAddr >> 22) & 0x3ff;
	}
	uint32_t getPTEIndex(uint32_t virtualAddr) {
		return (virtualAddr >> 12) & 0x3ff;
	}
	uint32_t getPageOffset(uint32_t virtualAddr) {
		return virtualAddr & 0xfff;
	}
	uint32_t getAddress(uint32_t entry) {
		return entry & 0xfffffc00;
	}
	uint32_t getVirtualAddr() {
		uint32_t virtualAddr = 0;
		__asm__("movl %%cr2, %0"
				:"=r"(virtualAddr));
		return virtualAddr;
	}

};

} // vm
} // mem
} // os

#endif