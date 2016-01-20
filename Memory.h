#include "types.h"
#include "Mapper.h"
#include "Mappers_lib.h"


#ifndef MEMORY_H
#define MEMORY_H


class Memory{
	private:
		Mapper mapper;
		uint8 *RAM;
		uint8 *ioRegs1;
		uint8 *ioRegs2;
		uint8 *xROM;
		uint8 *sRAM;
		uint8 *pgrROM;
		uint8 *map[0xFFFF];
	public:
		Memory();
		bool loadGame(string gameName);

		void write(int addr, uint8 value);
		void write(int addr, int value, int nbytes);
		uint8 read(int addr);
		int read(int addr, int nbytes);


}

#endif
