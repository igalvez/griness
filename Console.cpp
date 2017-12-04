#include "Console.h"
#include <time.h>
#include <signal.h>


bool Console::exit_run = false;

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

void Console::exit_handler(int sig){
	Console::exit_run = true;
}


void Console::run(){
	reset();
	clock_t start, end;
	float diff;
	signal(SIGINT, Console::exit_handler);
	start = clock();
	while(!Console::exit_run){
	//for(int i=0;i<262;i++){

		ppu->renderBackground();
		cpu->emulateCycles(110);//110 - 28820
		//ppu->showPatternTable(0x0000);
		//ppu->showNameTable(0x2000, 0x0000);

	}
	end = clock();
	diff = ((float)(end - start)/1000000.0F)*1000;
	printf("ONE FRAME TAKES APROX %lf miliseconds for the CPU\n", diff);
	
}
		


