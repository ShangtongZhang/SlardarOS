#ifndef __PLAIN_PAGE_POOL_HPP
#define __PLAIN_PAGE_POOL_HPP
#include "memory.h"
#include "vmController.hpp"
#include "test/assert.h"
#include "utils/stl.hpp"
#include "functional"

namespace os {
namespace mem {
namespace vm {

class PlainPagePool : public PagePool {
	constexpr static size_t PAGE_SIZE = VMController::PAGE_SIZE;
	using Page = uint8_t[PAGE_SIZE];
	Page* pages;
	size_t availablePage;
	os::map<size_t, std::reference_wrapper<const PageInfo>> pageInfos;


	size_t getAvailablePage() {
		assert(availablePage < nPages);
		return availablePage++;
	}
public:
	// TODO: Support lvalue invalidator
	PlainPagePool(size_t nPages_, const std::function<void(const PageInfo&)>&& invalidator_)
		: PagePool(nPages_, std::move(invalidator_)), availablePage(0) {
		pages = static_cast<Page*>(os::mem::memoryManager.allocateMemory(nPages * PAGE_SIZE, PAGE_SIZE));
	}

	void* getPage(const PageInfo& pageInfo) {
		size_t pageIndex = getAvailablePage();
		pageInfos.emplace(pageIndex, std::cref(pageInfo));
		return static_cast<void*>(&pages[pageIndex]);
	}

	~PlainPagePool() override {
		/**
		 * TODO: Invalidate all pages
		 */
		os::mem::memoryManager.freeMemory(static_cast<void*>(pages));
	}

};

} // vm
} // mem
} // os

#endif