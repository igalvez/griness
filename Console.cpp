#include "Console.h"


Console::Console(){
	memory = new Memory();
	cpu = new CPU();
}
		
void Console::loadGame(Cartridge cart){
	memory->loadGame(cart);
	cpu->initialize(memory);
}

void Console::reset(){
	cpu->reset();
}

void Console::run(){
	reset();
	while(cpu->emulateCycle());
}
		
