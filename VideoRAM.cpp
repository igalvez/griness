#include "VideoRAM.h"

uint8 VideoRAM::readVRam(uint16 addr){
	address = addr
	if (addr>=0x4000){
		address = addr%0x4000;
	}
	if (address<0x2000){
		return VRAM[address];
	}
	else if (addr<0x3f00){
	
	}
}

void VideoRAM::writeVRam(uint16 addr, uint8 value){

}
