#include "Cartridge.h"

bool Cartridge::loadGame(string gameName){
	FILE *newgame = fopen(gameName.c_str(), "rb");
	int size_game;
	struct stat st;


	if (!newgame){
		printf("Error: Couldnt open file \n");
		return false;
	}
	if (stat(gameName_c_str(), &st) !=0){
		printf("Error determining the file size \n");
		return false;
	}

	size_game = st.st_size;
		
	gameROM = new uint8[size_game];
	fread(gameROM, size_game*sizeof(uint8), 1, newgame);
	fclose(newgame);
	getMapper();
}


void Cartridge::getMapper(){

	int mapper_number = (gameROM[7] & 0xF0) | ((gameROM[6] & 0xF0) >> 4);	
	
	switch(mapper){
		case 1:
			mapper = MMC1();
		break;
	}
}
