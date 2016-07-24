#ifndef __PLAIN_MEMORY_MANAGER_HPP
#define __PLAIN_MEMORY_MANAGER_HPP
#include "memoryManager.hpp"
#include "new"

namespace os {
namespace mem {

namespace hidden {

class MemoryChunk {
public:
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

	MemoryChunk* split(size_t demanded) {
		size_t reserved = size - demanded;
		MemoryChunk* newChunk = reinterpret_cast<MemoryChunk*>(startAddr + reserved);
		// assert(reserved + sizeof(MemoryChunk) < size);
		new (newChunk) MemoryChunk(newChunk, demanded);
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
				MC* newMC = curMC->split(size);
				newMC->available = false;
				if (newMC->nextChunk == nullptr) {
					tailMemoryChunks = newMC;
				}
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

};

}
}

#endif