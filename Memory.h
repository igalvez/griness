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
		uint8 *map[0xFFFF];
		Memory();
		void loadGame(Cartridge &cart);
		void write(int addr, uint8 value);
		void write(int addr, int value, int nbytes);
		uint8 read(int addr);
		int read(int addr, int nbytes);


};

#endif
