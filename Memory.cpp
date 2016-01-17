#include "Memory.h"

Memory :: Memory(){
	// Internal RAM: 0000-0x7ffe
	for(int i=0; i<0x800;i++){
		map = new uint8;
	}
	
	// Mirrors of 0000-07ffe
	for (int i=0x800; i<0x2000; i++){
		j = i%0x800;
		map[i] = map[j];
	}

	// PPU Registers: 2000-2007
	for (int i=0x2000; i<0x2008; i++){
		map[i] = new uint8;
	}

	// Mirrors of 2000-2007
	for (int i=0x2008; i<0x4000; i++){
		j = (i - 0x2000)%8 ;
		map[i] = map[j];
	}
	
	// NES APU and I/O registers 4000-401f
	// Cartridge space 4020-bfe0

	// ROM should be from 8000 onwards
	
	for (int i=0x4000; i<=0xffff; i++){
		map[i] = new uint8;
	}
	

}

int Memory::read(int addr){
	return *map[map_chip.get_mem_address(this)];

}

int Memory::read(int addr, int nbytes){
	int vaddr = map_chip.get_mem_address(this);
	int aux = 0;

	for(int i=0;i<nbytes;i++){
		aux |= (*map[vaddr + i] << 8*i);
	}
	return aux;
}

void Memory::write(int addr, uint8 value){
	if(addr>=0x8000){ //ROM, give control to mapper
		
		mapper.write(
	}

	//int vaddr = map_chip.get_mem_address(this);
	//*map[vaddr] = value;

}

void Memory::write(int addr, uint8 value, int nbytes){
	int vaddr = map_chip.get_mem_address(this);

	if(bytes<=0 || nbytes>4){
		return
	}

	for(int i=0;i<nbytes;i++){
		*map[vaddr + i] = (value >> i*8) & 0x00FF;
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
			return true;
		}
		else{
			cout << "File too big!";
			return false;
		}
	}
	return false;
}
