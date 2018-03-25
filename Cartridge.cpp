#include "Cartridge.h"

bool Cartridge::loadGame(string gameName){
	FILE *newgame = fopen(gameName.c_str(), "rb");
	int size_game;
	struct stat st;


	if (!newgame){
		printf("Error: Couldnt open file \n");
		return false;
	}
	if (stat(gameName.c_str(), &st) !=0){
		printf("Error determining the file size \n");
		return false;
	}

	size_game = st.st_size;
		
	gameROM = new uint8[size_game];
	fread(gameROM, size_game*sizeof(uint8), 1, newgame);
	fclose(newgame);
	this->getMapper();
	n_banks = gameROM[4];
	chr_banks = gameROM[5];
	if ((gameROM[6]&0x08)==0x08){
		mirroring = 2;
	}
	else {
		mirroring = gameROM[6]&0x01;
	}
	return true;
}


void Cartridge::getMapper(){

	mapper_number = (gameROM[7] & 0xF0) | ((gameROM[6] & 0xF0) >> 4);	
	
	switch(mapper_number){
		//case 0:
			//mapper = NULL;
		case 1:
			printf("Mapper is MMC\n");
			mapper = MMC1();
		break;
	}
}
