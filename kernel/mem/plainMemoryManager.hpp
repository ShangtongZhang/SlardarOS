#ifndef __PLAIN_MEMORY_MANAGER_HPP
#define __PLAIN_MEMORY_MANAGER_HPP
#include "memoryManager.hpp"
#include "new"

namespace os {
namespace mem {

namespace hidden {

class MemoryChunk {
public:
	void* startAddr;
	size_t size;
	bool available;
	MemoryChunk* nextChunk;
	MemoryChunk* lastChunk;

	MemoryChunk(void* startAddr_, size_t size_, bool available_ = true) :
		startAddr(static_cast<void*>(static_cast<uint8_t*>(startAddr_) + sizeof(MemoryChunk))), 
		size(size_ - sizeof(MemoryChunk)),
		available(available_),
		nextChunk(nullptr),
		lastChunk(nullptr) {

	}

	MemoryChunk* split(size_t reserved) {
		MemoryChunk* newChunk = reinterpret_cast<MemoryChunk*>((static_cast<uint8_t*>(startAddr) + reserved));
		// assert(reserved + sizeof(MemoryChunk) < size);
		new (newChunk) MemoryChunk(newChunk, size - reserved);
		newChunk->lastChunk = this;
		newChunk->nextChunk = nextChunk;
		nextChunk = newChunk;
		if (newChunk->nextChunk != nullptr) {
			newChunk->nextChunk->lastChunk = newChunk;
		}
		size = reserved;
		return newChunk;
	}

	void* getMemoryPtr() {
		return startAddr;
	}

};

}

class PlainMemoryManager : public MemoryManager{
private:
	using MC = hidden::MemoryChunk;
	MC* pMemoryChunks;
public:
	PlainMemoryManager(void* startAddr, size_t size) {
		pMemoryChunks = new (startAddr) MC(startAddr, size);
	}

	void* allocateMemory(size_t size_) override {
		size_t size = size_ + sizeof(MC);
		MC* curMC = pMemoryChunks;
		while (curMC != nullptr) {
			if (curMC->available && curMC->size >= size) {
				MC* newMC = curMC->split(curMC->size - size);
				newMC->available = false;
				return newMC->getMemoryPtr();
			}
			curMC = curMC->nextChunk;
		}
		return nullptr;
	}

	void freeMemory(void* ptr) override {
		MC* curMC = reinterpret_cast<MC*>(static_cast<uint8_t*>(ptr) - sizeof(MC));
		curMC->available = true;
	}

};

}
}

#endif