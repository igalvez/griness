#include <sys/stat.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Mapper.h"

#include "mmc1.h"

using namespace std;

#ifndef CARTRIDGE_H
#define CARTRIDGE_H

class Cartridge {
	public:
		uint8 *gameROM;
		Mapper mapper;
		bool loadGame(string gameName);
		void getMapper();
};
		
#endif
