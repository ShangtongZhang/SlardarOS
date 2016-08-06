#ifndef __VIRTUAL_MEMORY_H
#define __VIRTUAL_MEMORY_H
#include "defs.h"

namespace os {
namespace mem {
namespace vm {

namespace hidden {

constexpr uint32_t ENTRY_PER_PAGE_DIR = 0x400;
constexpr uint32_t ENTRY_PER_PAGE_TABLE = 0x400;
constexpr uint32_t PAGE_SIZE = 0x1000;
constexpr uint32_t PAGE_NP = 0;
constexpr uint32_t PAGE_P = 1;
constexpr uint32_t PAGE_RW_R = 0;
constexpr uint32_t PAGE_RW_W = 2;
constexpr uint32_t PAGE_US_S = 0;
constexpr uint32_t PAGE_US_U = 4;

} // hidden

void initVM();

/**
 * Initialize a page directory, map all virtual address 
 * below @maxAddress to identical linear address, 
 * all other address will be set to "Not Present"
 * @return Address of the page directory
 */
uint32_t initPageDirectory(uint32_t maxAddress = END_OF_KENEL_SPACE);

} // vm
} // mem
} // os

#endif