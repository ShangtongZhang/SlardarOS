#ifndef __PLAIN_MEMORY_MANAGER_HPP
#define __PLAIN_MEMORY_MANAGER_HPP
#include "defs.h"
#include "test/assert.h"
#include "memoryManager.hpp"

#include "new"

namespace os {
namespace mem {

namespace hidden {

class MemoryChunk {
public:

	/**
	 * @startAddr and @size doesn't contain sizeof(MemoryChunk)
	 */
	uint8_t* startAddr;
	size_t size;

	bool available;
	MemoryChunk* nextChunk;
	MemoryChunk* lastChunk;

	MemoryChunk(void* startAddr_, size_t size_, bool available_ = true) :
		startAddr(static_cast<uint8_t*>(startAddr_) + sizeof(MemoryChunk)), 
		size(size_ - sizeof(MemoryChunk)),
		available(available_),
		nextChunk(nullptr),
		lastChunk(nullptr) {

	}

	// @demanded_: it should contain sizeof(MemoryChunk)
	MemoryChunk* split(size_t demanded_) {
		assert(demanded_ <= size);
		size_t demanded = demanded_ - sizeof(MemoryChunk);
		MemoryChunk* newChunk = reinterpret_cast<MemoryChunk*>(startAddr + demanded);
		new (newChunk) MemoryChunk(newChunk, size - demanded);
		newChunk->lastChunk = this;
		newChunk->nextChunk = nextChunk;
		nextChunk = newChunk;
		if (newChunk->nextChunk != nullptr) {
			newChunk->nextChunk->lastChunk = newChunk;
		}
		size = demanded;
		return this;
	}

	void* getMemoryPtr() {
		return static_cast<void*>(startAddr);
	}

};

}

class PlainMemoryManager : public MemoryManager{
private:
	using MC = hidden::MemoryChunk;
	MC* headMemoryChunks;
	MC* tailMemoryChunks;
public:
	PlainMemoryManager() : 
		headMemoryChunks(nullptr),
		tailMemoryChunks(nullptr) {

	}

	PlainMemoryManager(void* startAddr, size_t size) {
		headMemoryChunks = new (startAddr) MC(startAddr, size);
		tailMemoryChunks = tailMemoryChunks;
	}

	void* allocateMemory(size_t size_) override {
		size_t size = size_ + sizeof(MC);
		MC* curMC = headMemoryChunks;
		while (curMC != nullptr) {
			if (curMC->available && curMC->size >= size) {
				curMC->split(size);
				curMC->available = false;
				MC* newMC = curMC->nextChunk;
				if (newMC->nextChunk == nullptr) {
					tailMemoryChunks = newMC;
				}
				return curMC->getMemoryPtr();
			}
			curMC = curMC->nextChunk;
		}
		return nullptr;
	}

	void* allocateMemory(size_t size_, uint32_t alignment_) {
		MC* curMC = headMemoryChunks;
		while (curMC != nullptr) {
			if (curMC->available) {
				uint8_t* startAddr = curMC->startAddr;
				startAddr += sizeof(MC);
				uint32_t realStartAddr = reinterpret_cast<uint32_t>(startAddr);
				realStartAddr = (realStartAddr / alignment_ + 1) * alignment_;
				uint8_t* realEndAddr = reinterpret_cast<uint8_t*>(realStartAddr + size_ + sizeof(MC));
				if (curMC->startAddr + curMC->size >= realEndAddr) {
					assert(realStartAddr % alignment_ == 0);
					void* demandedMCAddr = reinterpret_cast<void*>(realStartAddr - sizeof(MC));
					assert(static_cast<uint8_t*>(demandedMCAddr) >= curMC->startAddr);
					MC* demandedMC = new (demandedMCAddr) MC (demandedMCAddr, size_ + sizeof(MC));
					assert(demandedMC);
					demandedMC->available = false;

					void* remainedMCAddr = reinterpret_cast<void*>(realStartAddr + size_);
					size_t remainedMCSize = curMC->startAddr + curMC->size - static_cast<uint8_t*>(remainedMCAddr);
					assert(remainedMCSize > sizeof(MC));
					MC* remainedMC = new (remainedMCAddr) MC (remainedMCAddr, remainedMCSize);
					assert(remainedMC);

					curMC->size = static_cast<uint8_t*>(demandedMCAddr) - curMC->startAddr;

					demandedMC->nextChunk = remainedMC;
					demandedMC->lastChunk = curMC;

					remainedMC->nextChunk = curMC->nextChunk;
					remainedMC->lastChunk = demandedMC;

					curMC->nextChunk = demandedMC;
					if (remainedMC->nextChunk != nullptr) {
						remainedMC->nextChunk->lastChunk = remainedMC;
					} else {
						tailMemoryChunks = remainedMC;
					}

					return demandedMC->getMemoryPtr();
				}
			}
			curMC = curMC->nextChunk;
		}
		return nullptr;

	}

	void freeMemory(void* ptr) override {
		MC* curMC = reinterpret_cast<MC*>(static_cast<uint8_t*>(ptr) - sizeof(MC));
		curMC->available = true;
	}

	void addMemory(void* addr, size_t size) override {
		MC* newMC = static_cast<MC*>(addr);
		new (newMC) MC(addr, size);
		if (headMemoryChunks == nullptr) {
			headMemoryChunks = tailMemoryChunks = newMC;
			return;
		}
		newMC->nextChunk = nullptr;
		newMC->lastChunk = tailMemoryChunks;
		tailMemoryChunks->nextChunk = newMC;
		tailMemoryChunks = newMC;
	}

	void reclaim() override {
		MC* curMC = headMemoryChunks;
		while (curMC != nullptr) {
			while (curMC != nullptr && !curMC->available) {
				curMC = curMC->nextChunk;
			}
			if (curMC == nullptr) {
				return;
			}
			MC* nextMC = curMC->nextChunk;
			if (nextMC == nullptr || !nextMC->available ||
				reinterpret_cast<uint8_t*>(nextMC) != static_cast<uint8_t*>(curMC->getMemoryPtr()) + curMC->size) {
				curMC = nextMC;
				continue;
			}
			curMC->size += nextMC->size + sizeof(MC);
			curMC->nextChunk = nextMC->nextChunk;
			if (curMC->nextChunk != nullptr) {
				curMC->nextChunk->lastChunk = curMC;
			} else {
				tailMemoryChunks = curMC;
			}
		}
	}

	uint32_t maxAddress() {
		return reinterpret_cast<uint32_t>(tailMemoryChunks->startAddr + tailMemoryChunks->size - 1);
	}

};

}
}

#endif