#ifndef __PLAIN_PAGE_POOL_HPP
#define __PLAIN_PAGE_POOL_HPP
#include "memory.h"
#include "vmController.hpp"

namespace os {
namespace mem {
namespace vm {

class PlainPagePool : public PagePool {
	constexpr static size_t PAGE_SIZE = VMController::PAGE_SIZE;
	using Page = uint8_t[PAGE_SIZE];
	Page* pages;
public:
	PlainPagePool(size_t nPages_, const std::function<void(const PageInfo&)>& invalidator_)
		: PagePool(nPages_, invalidator_) {
		pages = static_cast<Page*>(os::mem::memoryManager.allocateMemory(nPages * PAGE_SIZE, PAGE_SIZE));
	}

	void* getPage(const PageInfo&) {
		return nullptr;
	}

};

} // vm
} // mem
} // os

#endif