#ifndef __INFRASTRUCTURE_HPP
#define __INFRASTRUCTURE_HPP

namespace os {
namespace init {

enum GDTSelector {
	RPL0 = 0x0,
	RPL3 = 0x3,
	codeKernel = 0x8,
	dataKernel = 0x10,
	codeUser = 0x18,
	dataUser = 0x20,
	tssSeg = 0x28,
	systemCall = 0x30
};

class SegDescriptor {
private:
	uint32_t base;
	uint32_t limit;
	uint32_t attr;
public:
	SegDescriptor(uint32_t base_, uint32_t limit_, uint32_t attr_)
		: base(base_), limit(limit_), attr(attr_) {

	}

	operator uint64_t () {
		class __attribute__((__packed__)) {
		public:
			uint16_t limitLow;
			uint16_t baseLow;
			uint8_t baseMid;
			uint16_t attr;
			uint8_t baseHigh;
		} desc;
		desc.limitLow = static_cast<uint16_t>(limit & 0xffff);
		desc.baseLow = static_cast<uint16_t>(base & 0xffff);
		desc.baseMid = static_cast<uint8_t>((base & 0xff0000) >> 16);
		desc.baseHigh = static_cast<uint8_t>((base & 0xff000000) >> 24);
		desc.attr = static_cast<uint16_t>(attr & 0xf0ff);
		desc.attr |= static_cast<uint16_t>((limit & 0xf0000) >> 8);
		return *reinterpret_cast<uint64_t*>(&desc);
	}

	enum SegAttr {
		code = 0x98,
		dataRW = 0x92,
		dataRWA = 0x93,
		bit32 = 0x4000,
		granularity4K = 0x8000,
		tssSeg = 0x89,
		DPL0 = 0x00,
		DPL3 = 0x60
	};
};

class GateDescriptor {
private:
	uint16_t selector;
	uint32_t offset;
	uint8_t attr;
	uint8_t paramCount;
public:
	template <typename T>
	GateDescriptor(T* offset_, 
		uint16_t selector_ = static_cast<uint16_t>(os::init::GDTSelector::codeKernel),
		uint8_t attr_ = static_cast<uint8_t>(GateAttr::interruptionGate), 
		uint8_t paramCount_ = 0) :
		selector(selector_), 
		offset(reinterpret_cast<uint32_t>(offset_)), 
		attr(attr_), paramCount(paramCount_) {

	}

	operator uint64_t () {
		class __attribute__((__packed__)) {
		public:
			uint16_t offsetLow;
			uint16_t selector;
			uint16_t attr;
			uint16_t offsetHigh;
		} gate;
		gate.offsetLow = static_cast<uint16_t>(offset & 0xffff);
		gate.offsetHigh = static_cast<uint16_t>((offset >> 16) & 0xffff);
		gate.selector = selector;
		gate.attr = static_cast<uint16_t>(attr) << 8;
		gate.attr |= paramCount & 0x1f;
		return *reinterpret_cast<uint64_t*>(&gate);
	}

	enum GateAttr {
		interruptionGate = 0x8e,
		trapGate = 0x8f,
		callGate = 0x8c
	};
};

class __attribute__((__packed__)) TaskStateSegment {
public:
	uint32_t backLink;
	uint32_t espRing0;
	uint32_t ssRing0;
	uint32_t espRing1;
	uint32_t ssRing1;
	uint32_t espRing2;
	uint32_t ssRing2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t LDT;
	uint16_t debugTrapFlag;
	uint16_t IOMapBase;
};

} // init
} // os

#endif