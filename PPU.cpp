#include "PPU.h"
#include "Memory.h"

PPU::PPU(VideoRAM &RAM, Memory *mem){
	this->RAM = &RAM;
	
	
	for (int i=0; i<8; i++){
		regs[i] = map[0x2000 + i];
	}
	dmaReg = map[0x4014];
}


uint8 PPU::readVRam(uint16 addr){
	return RAM.readVram(addr);
}

void PPU::writeVRam(uint16 addr, uint8 value){
	RAM.writeVRam(addr,value);
}
	
