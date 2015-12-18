#include "Memory.h"

Memory :: Memory(){
	// Internal RAM
	for(int i=0; i<0x800;i++){
		map = new uint8;
	}
	
	// Mirrors of 0000-07ffe
	for (int i=0x800; i<0xfff; i++){
		j = i%0x800;
		map[i] = map[j];
	}

	// PPU Registers
	for (int i=0x2000; i<0x2007; i++){
		map[i] = new uint8;
	}

	// Mirrors of 2000-2007
	for (int i=0x2008; i<0x3fff; i++){
		j = (i - 0x2000)%8 ;
		map[i] = map[j];
	}
	
	// NES APU and I/O registers 4000-401f
	// Cartridge space 4020-bfe0
	
	for (int i=0x4000; i<0xffff; i++){
		map[i] = new uint8;
	}
	

}

int Memory::loadGame(string gameName){
	char *buffer;
	int size_game;

	ifstream newgame(gameName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	if (newgame.is_open()){
		size_game = newgame.tellg();
		newgame.seekg(0, ios::beg);
		
		if (size_game <= (4096-512)){
			buffer = new uint8[size_game];
			newgame.read(buffer, size_game);
			for(int i=0; i<size_game; i++){
				map[] = buffer[i];
			}
			return 0;
		}
		else{
			cout << "File too big!";
			return 1;
		}
	}
	return 1;
}
