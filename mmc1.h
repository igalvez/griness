#include "Mapper.h"
#include "types.h"

#define lowBank 0xC000;
#define highBank 0x8000;
#define BANK_SIZE 16*1024

#ifndef MMC1_H
#define MMC1_H

class MMC1: public Mapper {
	private:
		uint8 shift_reg;
		uint8 controlReg;
		int switchBank; // current bank to be switched, lowBank, highBank or 0 if both
	public:
		void clear();
		void write_control(int addr);
		int write(Memory *mem, int addr, uint8 value);
		void switch_banks(Memory *mem, int addr);
};
#endif
		
	
