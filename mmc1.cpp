#include "mmc1.h"


enum modesPrg {
	high, // switch higher bank
	low,  // switch lower bank
	switch32 // 32 KB switch
};

int MMC1::write(Memory *mem, int addr, uint8 value){
	if ((value&0x80)==0x80){ 
	// If bit 7 is set clear shift register
		clear();
		return 0;
	}
	shift_reg >>= 1;
	uint8 aux = (value&0x01) << 7;
	shift_reg |= aux;

	// Check if shift register is full (SR only has 5 bits. So only the 5 most significant bits of byte mathers.) xxxxx___
	if((shift_reg&0x04)==0x04){
		if(addr >= 0xA000){
			switch_banks(mem, addr);
		}
		else{
			write_control(addr);
		}
		
		clear();
	}	
}

void MMC1::clear(){
	shift_reg = 0x80;	
}

void MMC1::switch_banks(Memory *mem, int addr){
	int bank = (shift_reg >> 3); // xxxxx___ =>  ___xxxxx
	if(addr < 0xC000){
		//CHR bank 0 TODO
		chrBank[0] = bank & 0xFF;
	}
	else if(addr < 0xE000){
		//CHR bank 1 TODO
		chrBank[1] = bank & 0xFF;
	}
	else{
		//PRG bank TODO
		if(prgBank != bank&0x0F){ 
			prgBank = bank & 0x0F;

			if (switchBank!=0){
				int rom_addr = prgBank*BANK_SIZE;
				for (int aux=0; aux<BANK_SIZE; aux++){
					*mem->map[switchBank+aux] = *mem->cartridge->gameROM[rom_addr+aux];
				}

			}
			
		}	
	}
	
}

void MMC1::write_control(int addr){
}
