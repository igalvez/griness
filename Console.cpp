#include "Console.h"


Console::Console(){
	memory = new Memory();
	cpu = new CPU();
	cpu->initialize(memory);
}
		
void Console::loadGame(Cartridge cart){
	memory->loadGame(cart);
}

void Console::run(){
	while(cpu->emulateCycle());
}
		
