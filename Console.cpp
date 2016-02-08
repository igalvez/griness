#include "Memory.h"
#include "CPU.h"
#include "Cartridge.h"

#ifndef CONSOLE_CPP
#define CONSOLE_CPP

class Console {
	private:
		Memory *memory;
		CPU *cpu;

	public:
		Console(){
			memory = new Memory();
			cpu = new CPU(memory);
			cpu->initialize(&memory);
		}
		void loadGame(Cartridge cart){
			memory->loadGame(cart);
		}
		void run(){
			while(true){
				cpu->emulateCycle();
			}
		}
}

#endif
		
