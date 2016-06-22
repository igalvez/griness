#include "types.h"

#ifndef PPU_H
#define PPU_H

class Memory;
class PPU {
	private:
		VideoRAM *RAM;
		uint8 *dmaReg; // Located on CPU mem at addr $4014
		uint8 *regs[8]; // Addrs $2000-$2007 from CPU memory
	public:
		PPU(VideoRAM &RAM, Memory *mem);
		uint8 readVRam(uint16 addr);
		void writeVRam(uint16 addr, uint8 value);
}
