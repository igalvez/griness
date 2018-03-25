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
	//ppu->initialize(memory);
	cpu->initialize(memory);//, 0xFFFA);
	//memory->loadGame(cart);
}

void Console::reset(){
	cpu->reset();
}

void Console::exit_handler(int sig){
	Console::exit_run = true;
}


void Console::run(bool debug){
	reset();
	clock_t start, end;
	float diff;
	signal(SIGINT, Console::exit_handler);
	start = clock();
	long int cycles = 0;
	long int ncycles = 0;
	long int steps = 0;
	long int inst = 0;
	long int num = 110;
	int gotcycles = 0;
	int out = 0;

	while(!Console::exit_run){
	//for(int i=0;i<33;i++){
		//printf("teste\n");
		if (debug){
			scanf("%ld",&num);
			if (num==0){
				debug = false;
				num = 110;
			}
		}
		
		while(steps<num){
		//ppu->renderBackground();
			//cycles += gotcycles;//110 - 28820
			printf("\nCYCLES: %ld (+%d) / Instructions: %ld\n", cycles, gotcycles, inst);
			gotcycles = cpu->emulateCycles(1);
			if (gotcycles<0){
				out = 1;
				break;
			}
			inst++;
			steps++;
			cycles += gotcycles;
			ppu->emulateCycle(gotcycles*3);
			//printf("cycleS %ld\n\n", cycles);


		}
		/*if (num==1){
			cycles += gotcycles;//110 - 28820
			ncycles += gotcycles;
			gotcycles = cpu->emulateCycles(1);
			printf("ncycle %ld\n", ncycles);
			printf("cycleS %ld\n\n", cycles);
		}*/
		steps = 0;
		if(out){
			break;
		}
		//ppu->renderBackground();
		//cycles = cpu->emulateCycles(1);//110 - 28820

		//if(cycles<0){
		//	break;
		//}
		//ncycles += cycles;
		//ppu->showPatternTable(0x0000);
		//ppu->showNameTable(0x2000, 0x1000);
		//ppu->showNameTables();
		//ppu->emulateCycle(gotcycles*3);

	}
	end = clock();
	diff = ((float)(end - start)/1000000.0F)*1000;
	printf("ONE FRAME TAKES APROX %lf miliseconds for the CPU\n", diff);
	printf("cycles = %ld\n",cycles);
	
}
		


