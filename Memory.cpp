#include "Memory.h"

Memory :: Memory(){
	// Internal RAM: 0000-0x7ffe

	RAM = new uint8[0x800];
	int j;
	// Pointers to RAM and mirrors of 0000-07ffe (0x800-0x2000)
	for (int i=0; i<0x2000; i++){
		j = i%0x800;
		map[i] = &RAM[j];
	}

	// IO Registers: 2000-2007
	ioRegs1 = new uint8[8];

	// Pointers to I/O regs and mirrors to it (0x2008-0x3FFF)
	for (int i=0x2000; i<0x4000; i++){
		j = (i - 0x2000)%8 ;
		map[i] = &ioRegs1[j];
	}
	
	// NES APU and I/O registers 4000-401f
	ioRegs2 = new uint8[0x20];
	// Pointers to APU registers
	for (int i=0x4000; i<0x4020; i++){
		j = i - 0x4000;
		map[i] = &ioRegs2[j];
	}	


	// Expansion ROM
	xROM = new uint8[0x1fdf];
	// Pointers to expansion ROM
	for (int i=0x4020; i<0x6000; i++){
		j = i - 0x4020;
		map[i] = &xROM[j];
	}		

	// sRAM
	sRAM = new uint8[0x2000];
	// Pointers to expansion ROM
	for (int i=0x6000; i<0x8000; i++){
		j = i - 0x6000;
		map[i] = &sRAM[j];
	}	

	// PRG-ROM
	prgROM = new uint8[0x8000];
	for (int i=0x8000; i<0xffff; i++){
		j = i - 0x8000;
		map[i] = &prgROM[j];
	}	
	
	// Cartridge space 4020-bfe0

	// ROM should be from 8000 onwards
	

}

uint8 Memory::read(int addr){
	return *map[addr];

}

int Memory::read(int addr, int nbytes){
	int aux = 0;

	for(int i=0;i<nbytes;i++){
		aux |= (*map[addr + i] << 8*i);
	}
	return aux;
}

void Memory::write(int addr, uint8 value){
	if(addr>=0x8000){ //ROM, give control to mapper
		cartridge->mapper.write(this,addr,value);
	}
	else {
		*map[addr] = value;
	}
}


void Memory::write(int addr, int value, int nbytes){
	//int vaddr = map_chip.get_mem_address(this);

	if(nbytes<=0 || nbytes>4){
		return;
	}

	for(int i=0;i<nbytes;i++){
		*map[addr + i] = (value >> i*8) & 0x00FF;
	}

}

void Memory::loadGame(Cartridge &cart){
	cartridge = &cart;
	int j = 0;
	for (int i=0; i<(0xFFFF-0x8000); i++){
		*map[i+0x8000] = cartridge->gameROM[i+528];
		j++;
	}
}

