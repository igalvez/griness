#include "VideoRAM.h"

uint8 VideoRAM::readVRam(uint16 addr){
	address = addr
	if (addr>=0x4000){
		address = addr%0x4000;
	}
	if (address<0x3000){
		//address = address%0x3000
		return VRAM[address];
	}
	else if(address<0x3f00){
		address = address%0x3000;
	}
	else if(address<0x3f20){
		return VRAM[address];
	}
	else if(address<0x4000){
		address = (address%0x3f20) + 0x3f00;
	}
	else{
		address = 0;
	}
	return VRAM[address];
}

void VideoRAM::writeVRam(uint16 addr, uint8 value){
	address = addr
	if (addr>=0x4000){
		address = addr%0x4000;
	}
	if (address<0x3000){
		//address = address%0x3000
		VRAM[address] = value;
	}
	else if(address<0x3f00){
		VRAM[address%0x3000] = value;
	}
	else if(address<0x3f20){
		VRAM[address] = value;
	}
	else if((address<0x4000){
		VRAM[(address%0x3f20) + 0x3f00] = value;
	}
	//return VRAM[address];
}
