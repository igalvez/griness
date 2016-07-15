#include "Memory.h"
#include "CPU.h"
#include "PPU.h"
#include "Cartridge.h"
#include "types.h"

#ifndef CONSOLE_H
#define CONSOLE_H

class Console {
	private:
		Memory *memory;
		CPU *cpu;
		PPU *ppu;

	public:
		Console();
		void loadGame(Cartridge cart);
		void run();
		void reset();
};

#endif
		
