#ifndef __INITIALIZATION_H
#define __INITIALIZATION_H
#include "defs.h"

namespace os {
namespace init {

class GDTDescriptor {
private:
	uint32_t base;
	uint32_t limit;
	uint32_t attr;
public:
	GDTDescriptor(uint32_t base_, uint32_t limit_, uint32_t attr_)
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

	enum GDTAttr {
		code = 0x98,
		dataRW = 0x92,
		bit32 = 0x4000,
		granularity4K = 0x8000,
		DPL0 = 0x00,
		DPL3 = 0x60 
	};
};

void initBSS();
void initGDT();

enum GDTSelector {
	codeKernel = 0x8,
	dataKernel = 0x10,
	codeUser = 0x18,
	dataUser = 0x20
};

} // init
} // os

#endif