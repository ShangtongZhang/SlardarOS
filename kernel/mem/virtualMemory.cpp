#include "virtualMemory.h"
#include "memoryManage.h"
#include "intr/interruption.h"

uint32_t initPageDirectory(uint32_t maxAddress) {
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
	PTE = 0 | PAGE_P | PAGE_US_S | PAGE_RW_W;
	for (size_t i = nPTE; i < nPageTables * ENTRY_PER_PAGE_TABLE; ++i) {
		pageTable[i] = PTE;
	}
	return pageDirBaseAddr;
}

void initVirtualMemory() {
	uint32_t maxAddress = os::mem::memoryManager.maxAddress();
	uint32_t pageDirAddr = initPageDirectory(maxAddress);
	__asm__("movl %0, %%eax;"
			"movl %%eax, %%cr3;"
			"movl %%cr0, %%eax;"
			"orl $0x80000000, %%eax;"
			"movl %%eax, %%cr0;"
			::"r"(pageDirAddr)
			:"%eax");
	__asm__("movl 0x4fffffff, %%eax"
			:::"%eax");
}

void pageFaultHandler() {
	ISR_RETURN;
}