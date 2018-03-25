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

/* gameROM stores the ROM data. There is a 16 byte header at the start which is described below:
   =========================================================================================================
	START BYTE | LENGTH(BYTES)| CONTENTS
   =========================================================================================================
		0      |     3		  |  Should contain the string NES to identify the file as an iNES file
    --------------------------------------------------------------------------------------------------------
		3      |	 1        |  Should contain the value $1A, also used to identify the format
    --------------------------------------------------------------------------------------------------------
		4      |     1        |  Number of 16 KB PRG-ROM banks
    --------------------------------------------------------------------------------------------------------
		5      |     1        |  Number of 8 KB CHR-ROM/VROM banks (Graphics information)
    --------------------------------------------------------------------------------------------------------
		6      |     1        |  ROM Control Byte 1:
			   |              |    - Bit 0: Type of mirroring. 0 for horizontal, 1 for vertical
			   |              |    - Bit 1: Indicate presence of batery-packed RAM at mem $6000-$7FFF
			   |              |    - Bit 2: Indicate presence of a 512-byte trainer at mem $7000-$71FF
			   |              |    - Bit 3: If set, it overrides bit 0 to indicate four screen mirroring
			   |              |    - Bit 4-7: Four lower bits of the mapper number
	--------------------------------------------------------------------------------------------------------
		7      |     1        |  ROM Control Byte 2:
			   |              |    - Bits 0-3: Reserved for future usage and should all be 0
			   |              |    - Bits 4-7: Four upper bits of the mapper number
	--------------------------------------------------------------------------------------------------------
		9      |     7        | Reserved for future usage and should all be 0
   =========================================================================================================
*/

class Cartridge {
	public:
		int n_banks;
		int chr_banks;
		uint8 *gameROM;
		Mapper mapper;
		uint8 mapper_number;
		uint8 mirroring;
		bool loadGame(string gameName);
		void getMapper();
};
		
#endif
