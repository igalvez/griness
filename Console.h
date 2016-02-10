#include "Memory.h"
#include "CPU.h"
#include "Cartridge.h"

#ifndef CONSOLE_H
#define CONSOLE_H

class Console {
	private:
		Memory *memory;
		CPU *cpu;

	public:
		Console();
		void loadGame(Cartridge cart);
		void run();
};

#endif
		
