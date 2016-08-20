#ifndef __VM_CONTROLLER_HPP
#define __VM_CONTROLLER_HPP
#include "defs.h"
#include "pagePool.hpp"
#include "memory"
#include "functional"

namespace os {
namespace mem {
namespace vm {

class VMController {
	using PageInfo = PagePool::PageInfo;
protected:
	std::unique_ptr<PagePool> pagePool;
	uint32_t kernelPageDir;
	uint32_t targetPageDir;
	uint32_t targetAddress;
public:
	template <typename PagePoolType>
	void initPagePool(size_t pagePoolSize) {
		std::function<void(const PageInfo&)> invalidator = [this](const PageInfo& pageInfo) {
			this->invalidatePage(pageInfo);
		};
		pagePool.reset(new PagePoolType(pagePoolSize, invalidator));
	}

	void pageFaultHandler(uint32_t) {
		targetAddress = getVirtualAddr();
		targetPageDir = retrivePageDir();
		applyPageDir(kernelPageDir);
		uint32_t* pageDir = reinterpret_cast<uint32_t*>(targetPageDir);
		uint32_t PDEIndex = getPDEIndex(targetAddress);
		PDEHandler(pageDir, PDEIndex);
		uint32_t PDE = pageDir[PDEIndex];
		uint32_t* pageTable = reinterpret_cast<uint32_t*>(getAddress(PDE));
		uint32_t PTEIndex = getPTEIndex(targetAddress);
		PTEHandler(pageTable, PTEIndex);
		applyPageDir(targetPageDir);
	}

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
	virtual void invalidatePage(const PageInfo&) = 0;
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

	virtual ~VMController() {}
};

} // vm
} // mem
} // os

#endif