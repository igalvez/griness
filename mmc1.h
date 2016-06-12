#include "Mapper.h"
#include "types.h"

#define lowBank 0xC000;
#define highBank 0x8000;
#define BANK_SIZE 16*1024

#ifndef MMC1_H
#define MMC1_H

class Memory;

class MMC1: public Mapper {
	private:
		uint8 shift_reg;
		uint8 controlReg;
		/*
			Control Reg Legenda:
			bit0: Selects mirroring betweeen horizontal(0) or vertical (1)
			bit1: If set to 0 causes single screen mirroring
			bit2: If 0, PRG-ROM swapped at $C000. If 1, PRG-ROM swapped at $8000
			bit3: If 0, swap 32 KB of PRG-ROM at $8000. If 1, swap 16KB at address specified
				  by bit 2
			bit4: If the cartridge has VROM, 0 indicates swapping 8 KB of VROM 
				  at PPU $0000, 1 indicates swapping two 4 KB VROM pages at PPU 
				  $0000 and $1000. On 1024 KB cartridges this bit specifies whether to 
				  use 256 KB selection register 1.  
			bit7: Set to 1 to reset register
		*/
		uint8 chrBank[2]; // Register: CHR bank 0 and 1 to change
		uint8 prgBank; // Register: PRG bank to change
		int switchBank; // current bank to be switched, lowBank, highBank or 0 if both
	public:
		void clear();
		void write_control(int addr);
		int write(Memory *mem, int addr, uint8 value);
		void switch_banks(Memory *mem, int addr);
};
#endif
		
	
