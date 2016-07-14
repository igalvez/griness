#include "types.h"

#ifndef PPU_H
#define PPU_H

class Memory;
class PPU {
	private:
		VideoRAM *RAM;
		uint8 *dmaReg; // Located on CPU mem at addr $4014
		uint8 *regs[8]; // Addrs $2000-$2007 from CPU memory
		uint16 current_addr; // VRAM rendering current address

		// Address of top left corner tile (where to start rendering)
		uint16 start_addr; 
		uint8 write_toggle; // First or second write (should use only one bit)

		uint8 x_pixel;
		
	public:
		PPU(VideoRAM &RAM, Memory *mem);
		uint8 readVRam(uint16 addr);
		void writeVRam(uint16 addr, uint8 value);
}
