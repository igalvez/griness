#include <stdio.h>
#include <string>
#include "Console.h"
#include "Cartridge.h"



int main(int argc,char *argv[]){
	if (argc<2){
		printf("Please provide the full path of the game\n");
		return 1;
	}
	
	string gameName = argv[1];
	Cartridge game;
	game.loadGame(gameName);
	Console nes;
	nes.loadGame(game);
	nes.run();
	return 0;
}
