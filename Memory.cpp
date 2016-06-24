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
	for (int i=0x8000; i<=0xffff; i++){
		j = i - 0x8000;
		map[i] = &prgROM[j];
	}	
	
	// Cartridge space 4020-bfe0

	// ROM should be from 8000 onwards
	

}

//TODO: change the read and write to use this instead of map (which is an unnecessary waste of memory)
/*
uint8 Memory::read(uint16 addr){

*/

uint8 Memory::read(uint16 addr){
	return *map[addr];

}

int Memory::read(uint16 addr, int nbytes){
	int aux = 0;

	for(int i=0;i<nbytes;i++){
		aux |= (*map[addr + i] << 8*i);
	}
	return aux;
}

void Memory::write(int addr, sint8 value){
	if(addr>=0x8000){ //ROM, give control to mapper
		cartridge->mapper.write(this,addr,value);
	}
	else {
		*map[addr] = value;
		
		if(addr==0x4014){
			doDMA(value);
		}
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
	int lastBank = (cartridge->n_banks - 1)*BANK_SIZE + 16;
	
	for(int i=0; i<BANK_SIZE + 16; i++){
		*map[i+0x8000] = cartridge->gameROM[i+16];
	}

	for(int i=0; i<BANK_SIZE; i++){
		*map[i+0xC000] = cartridge->gameROM[i+lastBank];
	}

	//int z = cartridge->gameROM[lastBank + BANK_SIZE*4];
	//printf(" z = %x \n", z);

}

int Memory::switchBanks(int bank,uint8 mode){
	if (bank>cartridge->n_banks){
		printf("Error, bank number bigger than max number of banks in ROM");
		return 1;
	}
	//int addr = bank*BANK_SIZE;
	if (mode&0x02==0){ //switch 32KB
		int addr = (bank & 0x0E)*BANK_SIZE + 16; //ignore last bit (which is only used for 16KB bank switches)
		for(int i=0; i<2*BANK_SIZE; i++){
			*map[i+0x8000] = cartridge->gameROM[addr + i];
		}
	}
	else{
		int addr = bank*BANK_SIZE + 16;
		uint16 start_addr;
		if (mode&0x01==0){
			// Switch 16 KB at C000
			start_addr = 0xC000;
		}
		else{
			// Switch 16 KB at 8000
			start_addr = 0x8000;
		}
		for (int i=0; i<BANK_SIZE; i++){
			*map[i+start_addr] = cartridge->gameROM[addr + i];
		}
	}
	return 0;
}
			
int Memory::doDMA(uint8 value){
	uint16 addr = value*0x100;
	
	for(int i =0; i<256; i++){
		vram->SPRRAM[i] = *map[addr + i];
	}
}
	
