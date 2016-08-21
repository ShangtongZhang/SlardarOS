#ifndef __PLAIN_VM_CONTROLLER_HPP
#define __PLAIN_VM_CONTROLLER_HPP
#include "vmController.hpp"
#include "memory.h"
#include "pagePool.hpp"
#include "test/assert.h"

namespace os {
namespace mem {
namespace vm {

class PlainVMController: public VMController {
private:
	using PageInfo = PagePool::PageInfo;
	uint32_t initPageDirectory(uint32_t maxAddress) {
		using namespace hidden;
		uint32_t nPages = maxAddress / PAGE_SIZE + 1;
		uint32_t nPTE = nPages;
		uint32_t nPageTables = nPTE / ENTRY_PER_PAGE_TABLE + 1;
		uint32_t nPDE = nPageTables;
		size_t pagingInfoSize = (1 + nPDE) * PAGE_SIZE;
		uint32_t pageDirBaseAddr = reinterpret_cast<uint32_t>(
			os::mem::memoryManager.allocateMemory(pagingInfoSize, PAGE_SIZE));
		uint32_t pageTableBaseAddr = pageDirBaseAddr + PAGE_SIZE;
		uint32_t* pageDir = reinterpret_cast<uint32_t*>(pageDirBaseAddr);
		uint32_t PDE = pageTableBaseAddr | PAGE_P | PAGE_US_S | PAGE_RW_W;
		for (size_t i = 0; i < nPDE; ++i) {
			pageDir[i] = PDE;
			PDE += PAGE_SIZE;
		}
		assert(pageDirBaseAddr + pagingInfoSize == (PDE & 0xfffff000));
	
		// All PDEs and PTEs must live in kernel space
		assert(pageDirBaseAddr + pagingInfoSize < END_OF_KENEL_SPACE);
	
		PDE = PAGE_NP | PAGE_US_S | PAGE_RW_W;
		for (size_t i = nPDE; i < ENTRY_PER_PAGE_DIR; ++i) {
			pageDir[i] = PDE;
		}
		uint32_t* pageTable = reinterpret_cast<uint32_t*>(pageTableBaseAddr);
		uint32_t PTE = 0 | PAGE_P | PAGE_US_S | PAGE_RW_W;
		for (size_t i = 0; i < nPTE; ++i) {
			pageTable[i] = PTE;
			PTE += PAGE_SIZE;
		}
		PTE = 0 | PAGE_NP | PAGE_US_S | PAGE_RW_W;
		for (size_t i = nPTE; i < nPageTables * ENTRY_PER_PAGE_TABLE; ++i) {
			pageTable[i] = PTE;
		}
		return pageDirBaseAddr;
	}
public:
	uint32_t createKernelPageDir() override {
		uint32_t maxAddress = os::mem::memoryManager.maxAddress();
		return initPageDirectory(maxAddress);
	}

	uint32_t createUserPageDir() override {
		uint32_t maxAddress = END_OF_KENEL_SPACE - 1;
		return initPageDirectory(maxAddress);
	}

	void PDEHandler(uint32_t* pageDir, uint32_t PDEIndex) override {
		uint32_t PDE = pageDir[PDEIndex];
		if (PDE & PAGE_P) {
			return;
		}
		uint32_t* pageTable = static_cast<uint32_t*>(pagePool->getPage(
			PageInfo(targetAddress, targetPageDir, PageInfo::Flag::pageTable)));
			// os::mem::memoryManager.allocateMemory(PAGE_SIZE, PAGE_SIZE));
		uint32_t PTE = 0 | PAGE_NP | PAGE_US_S | PAGE_RW_W;
		for (size_t i = 0; i < ENTRY_PER_PAGE_TABLE; ++i) {
			pageTable[i] = PTE;
		}
		PDE |= reinterpret_cast<uint32_t>(pageTable);
		PDE |= PAGE_P | PAGE_TO_FREE;
		pageDir[PDEIndex] = PDE;
	}

	void PTEHandler(uint32_t* pageTable, uint32_t PTEIndex) override {
		uint32_t PTE = pageTable[PTEIndex];
		if (PTE & PAGE_P) {
			return;
		}
		uint8_t* page = static_cast<uint8_t*>(pagePool->getPage(
			PageInfo(targetAddress, targetPageDir)));
			// os::mem::memoryManager.allocateMemory(PAGE_SIZE, PAGE_SIZE));
		PTE |= reinterpret_cast<uint32_t>(page);
		PTE |= PAGE_P | PAGE_TO_FREE;
		pageTable[PTEIndex] = PTE;
	}

	void invalidatePage(const PageInfo& pageInfo) override {
		uint32_t vAddr = pageInfo.virtualAddr;

		/**
		 * For now only ordinary page can be invalidated.
		 * Invalidating page of page table isn't allowed.
		 */
		assert(pageInfo.flags & PageInfo::Flag::page);

		uint32_t* pageDir = reinterpret_cast<uint32_t*>(pageInfo.pageDir);
		uint32_t PDEIndex = getPDEIndex(vAddr);
		uint32_t PDE = pageDir[PDEIndex];
		uint32_t* pageTable = reinterpret_cast<uint32_t*>(getAddress(PDE));
		uint32_t PTEIndex = getPTEIndex(vAddr);
		pageTable[PTEIndex] &= ~PAGE_P;
	}
	
};

} // vm
} // mem
} // os

#endif