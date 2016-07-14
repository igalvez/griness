#include "PPU.h"
#include "Memory.h"

PPU::PPU(VideoRAM &RAM, Memory *mem){
	this->RAM = &RAM;
	
	
	for (int i=0; i<8; i++){
		regs[i] = map[0x2000 + i];
	}
	dmaReg = map[0x4014];
}


uint8 PPU::readVRam(uint16 addr){
	return RAM.readVram(addr);
}

void PPU::writeVRam(uint16 addr, uint8 value){
	RAM.writeVRam(addr,value);
}


void PPU::render(){
}


void PPU::executeNMI(){
	x_pixel = 0;
}


uint8 PPU::getFromNametable(uint16 addr){

}

void PPU::renderBackground(){

	/* VRAM address registers:
        FEDC BA98 7654 3210                  
		.yyy NNYY YYYX XXXX
	*/
	uint16 tile_addr;
	uint8 tile_low;
	uint8 tile_high;
	uint8 temp_black_and_white;
	uint8 temp;
	uint16 pattern_table_start;
	uint16 row_start_addr;

	//uint16 ppuaddr = 0x2000 + (start_addr&0x03FF);
	current_addr = start_addr;
	row_start_addr = start_addr;
	//uint16 temp;

	for(int row=0; row<FRAME_HEIGHT; row++){
		//nametable_addr = current_addr&0x07FF;

		while((current_addr&0xF000)<=0x7000){  // y fine scroll corresponding to scanline
		//for(int scanline=0; scanline<8; scanline++){//8 scanlines per tile row
			uint8 y = (current_addr&0x7000)>>12; // pixel line of tile
			screen_coord_y = row + y;
			for(int tile=0; tile<FRAME_WIDTH; tile++){
				tile_addr = readVRam(current_addr&0x07FF);
				tile_low = readVRam(pattern_table_start+y);
				tile_high = readVRam(pattern_table_start+y+8);
				temp_black_and_white = tile_low | tile_high;


				while(x_pixel<8){   // x fine scroll (pixel level)
					temp = temp_black_and_white&0x01;
					temp_black_and_white>>1;
					screen_coord_x =  tile + x_pixel;
					SDLBASE::renderPixel(temp,screen_coord_x,screen_coord_y); //TODO
					x_pixel++;
				}

				x_pixel = 0;
				if (current_addr&0x001F==0x001F){ // If reached end of nametable, clean x scroll and switch bit 10 (nametable)
					current_addr&=0xFFE0; 
					current_addr = (~current_addr&0x0400) | current_addr&0xFBFF;
				}
				else{
					current_addr++;
				}

			}
			//Set x back to initial coordinates so we can start at the beginning of next scanline
			current_addr = (start_addr&0x001F) | (current_addr&0xFFE0); 
			current_addr = (~current_addr&0x0400) | current_addr&0xFBFF; //switch bit 10 to go back to initial nametable
			current_addr += 0x1000; // Increment y fine scroll (scanline)
		}
		if(current_addr&0x03E0==0x03E0){ // If last row of nametable
			// Reset Y scroll and switch bit 11 (nametable)
			current_addr&=0xFC1F; 
			current_addr = (~current_addr&0x0800) | current_addr&0xF7FF;
				
		}
		else{
			current_addr+=0x0020;
		}

	}
}


/*
	//for(int row=0; row<FRAME_HEIGHT; row++){
	while(true){
		//scanline = current_addr&0x7000)
		//current_addr = current_add&0x0FFF;  // Set y scroll to 0
		nametable_addr = current_addr&0x07FF;

		while((current_addr&0xF000)<=0x7000){  // y fine scroll corresponding to scanline
		//for(int scanline=0; scanline<8; scanline++){//8 scanlines per tile row
			uint8 y = (current_addr&0x7000)>>12; // pixel line of tile
			while(true){
			//for(int tile=0; tile<FRAME_WIDTH; tile++){
				tile_addr = readVRam(nametable_addr);
				tile_low = readVRam(pattern_table_start+y);
				tile_high = readVRam(pattern_table_start+y+8);
				temp_black_and_white = tile_low | tile_high;
				
				while(x_pixel<8){   // x fine scroll (pixel level)
					temp = temp_black_and_white&0x01;
					temp_black_and_white>>1;
					SDLBASE::renderPixel(temp);
					x_pixel++;
				}
				x_pixel = 0;
				if (current_addr==0x001F){ //When reach 31 break from loop (End of nametable was reached)
					break;
				}
				current_addr += 1;
			}
			current_addr += 0x1000; // Increase fine y scroll

			// switch bit 10 
			current_addr = (~current_addr&0x0400) | current_addr&0xFBFF;
			
			// Set X scroll to 0, as it reached end of nametable
			current_addr &=0xFF70;
		}
		current_addr = current_add&0x0FFF

		if((current_add==0x0370){  
			break;
		}
		current_addr += 0x0020;  // Increase Y scroll

	} 
	// Switch bit 11 
	current_addr = (~current_addr&0x0800) | current_addr&0xF7FF;
	// Set Y scroll to 0, as end of nametable was reached
	current_addr&=0xFE1F;

















	// This is for horizontal scrolling

	if (horizontal_scroll){ //TODO
		//for(int i=0;i<FRAME_SIZE; i++){ // for each tile in frame
		for (int scanline=0; scanline<FRAME_SIZE; scanline++){
			for (int ntile=0; ntile<SCANLINE_SIZE; ntile++){
				
				if x_pixel == 7: //	Next tile
					x_pixel = 0;
					current_add++;
					if(current_addr&0x001F==0x001F){
						current_addr = (current_addr&0xFFE0) | 0x0400;    // Switch Nametable
					}
					else{
						current_addr++;
					}
				}
		
			

			temp = current_addr + 1;
			if(temp&0x001F==0x001F){
				v = (v&0xFFE0) | 0x0400;    // Switch Nametable
			}
			else{
				v++;
			}
		}

	}
	// This is for vertical scrolling //

	else{
			
	}	*/
			
}
	
