#include "Memory.h"
#include "CPU.h"
#include "Cartridge.h"

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
		void loadGame(Cartridge &cart){
			memory->cartridge = cart
		}
		void run(){
			while(true){
				cpu->emulateCycle();
			}
		}
}
		
