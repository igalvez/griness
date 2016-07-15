#include "types.h"

#ifndef VideoRAM_H
#define VideoRAM_H

class VideoRAM{
	private:
		uint8 VRAM[PPU_SIZE];
	public:
		uint8 SPRRAM[256];
		uint8 readVRam(uint16 addr);
		void writeVRam(uint16 addr, uint8 value);
};

#endif
