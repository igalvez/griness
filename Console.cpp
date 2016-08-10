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
		ppu->renderBackground();
		cpu->emulateCycles(114);
		ppu->showPatternTable(0x1000);
		//ppu->showNameTable(0x2400, 0x0000);
	}
}
		

