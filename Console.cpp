#include "Console.h"


Console::Console(){
	
	cpu = new CPU();
	ppu = new PPU();
	memory = new Memory(ppu);
}
		
void Console::loadGame(Cartridge cart){
	memory->loadGame(cart);
	ppu->initialize(memory);
	cpu->initialize(memory);//, 0xFFFA);
}

void Console::reset(){
	cpu->reset();
}

void Console::run(){
	reset();
	while(1){
		cpu->emulateCycles(29780);
		ppu->renderBackground();
	}
}
		

