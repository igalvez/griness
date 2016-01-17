
void MMC1::write(int addr, uint8 value){
	if ((value&0x80)==0x80){ 
	// If bit 7 is set clear shift register
		clear();
		return;
	}

	shift_reg >>= 1;
	uint8 aux = (value&0x01) << 7;
	shift_reg |= aux;

	// Check if shift register is full
	if((shift_reg&0x04)==0x04){
		if(addr > 0xA000){
			switch_banks(int addr);
		}
		else{
			
		}

		clear();
	}	
}

void MMC1::clear(){
	shitf_reg = 0x80;
	
}

void MMC1::switch_banks(){
	//bank = (shift_reg >> 3) & 0x0F;
	if(addr < 0xC000){
		//CHR bank 0 TODO
		chrBank[0] = (shift_reg >> 3) & 0xFF;
	}
	else if(addr < 0xE000){
		//CHR bank 1 TODO
		chrBank[1] = (shift_reg >> 3) & 0xFF;
	}
	
}
