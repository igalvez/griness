#include <stdio.h>
#include <string>
#include "Console.h"
#include "Cartridge.h"



int main(int argc,char *argv[]){
	bool debug = false;
	if (argc<2){
		printf("Please provide the full path of the game\n");
		return 1;
	}
	else if(argc>2){
		string par = argv[2];
		if (par[0] == 'd'){
			debug = true;
			printf("Debug on \n");
			printf("continue ...\n");
			getchar();
		}
	}

	string gameName = argv[1];
	Cartridge game;
	game.loadGame(gameName);
	Console nes;
	nes.loadGame(game);
	nes.run(debug);
	return 0;
}
