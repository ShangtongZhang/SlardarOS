#ifndef __PAGE_POOL_HPP
#define __PAGE_POOL_HPP
#include "defs.h"
#include "functional"

namespace os {
namespace mem {
namespace vm {

class PagePool {
public:
	class PageInfo {
	public:
		uint32_t virtualAddr;
		uint32_t pageDir;
		uint32_t flags;
		enum Flag {
			page = 0x1,
			pageTable = 0x2
		};
		PageInfo(uint32_t virtualAddr_, uint32_t pageDir_, uint32_t flags_ = Flag::page) 
			: virtualAddr(virtualAddr_), pageDir(pageDir_), flags(flags_) {
	
		}
	};
protected:
	// Real number of pages
	size_t nPages;
	const std::function<void(const PageInfo&)>& invalidator;
public:



	PagePool(size_t nPages_, const std::function<void(const PageInfo&)>& invalidator_)
		: nPages(nPages_), invalidator(invalidator_) {

	}

	virtual void* getPage(const PageInfo&) = 0;
	virtual ~PagePool() {}
};

} // vm
} // mem
} // os

#endif