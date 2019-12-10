#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Memory.h"
#include "CPU.h"
#include "PPU.h"
#include "Cartridge.h"
#include "types.h"

//using namespace std;

#ifndef CONSOLE_H
#define CONSOLE_H



class Console {
	private:
		Memory *memory;
		CPU *cpu;
		PPU *ppu;		
		static void exit_handler(int sig);

	public:
		static bool exit_run;
		//std::ofstream outfile;
		Console();
		void loadGame(Cartridge cart);
		void run(bool debug=false);
		void reset();
};

#endif
		
