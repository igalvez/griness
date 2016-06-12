#include "types.h"
#include "Mapper.h"
#include "Cartridge.h"
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

#ifndef MEMORY_H
#define MEMORY_H


class Memory{
	private:
		//Mapper mapper;
		
		uint8 *RAM;
		uint8 *ioRegs1;
		uint8 *ioRegs2;
		uint8 *xROM;
		uint8 *sRAM;
		uint8 *prgROM;
	public:
		Cartridge *cartridge; //cartridge
		uint8 *map[0x10000];
		Memory();
		void loadGame(Cartridge &cart);
		void write(int addr, sint8 value);
		void write(int addr, int value, int nbytes);
		uint8 read(uint16 addr);
		int read(uint16 addr, int nbytes);
		int switchBanks(int bank, uint8 mode);


};

#endif
