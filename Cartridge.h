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
		int n_banks;
		int chr_banks;
		uint8 *gameROM;
		Mapper mapper;
		uint8 mapper_number;
		bool loadGame(string gameName);
		void getMapper();
};
		
#endif
