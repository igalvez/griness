#include "PPU.h"
#include "Memory.h"

unsigned int PPU::palette_map[64] ={

    //0      , 1       , 2       , 3       , 4       , 5       , 6       , 7       , 8       , 9       , A       , B       , C       , D       , E       , F
	 0x757575, 0x271b8f, 0x0000ab, 0x47009f, 0xab0073, 0xab0013, 0xa70000, 0x7f0b00, 0x432f00, 0x004700, 0x005100, 0x003f17, 0x1b3f3f, 0x000000, 0x000000, 0x000000, // 0
	 0xbcbcbc, 0x0073ef, 0x233bef, 0x8300f3, 0xbf00bf, 0xe7005b, 0xdb2b00, 0xcb4f0f, 0x8b7300, 0x009700, 0x00ab00, 0x00933b, 0x00838b, 0x000000, 0x000000, 0x000000, // 1
	 0xffffff, 0x3fbfff, 0x5f97ff, 0xa78bfd, 0xf77bff, 0xff77b7, 0xff7763, 0xff9b3b, 0xf3bf3f, 0x83d313, 0x4fdf4b, 0x58f898, 0x00ebdb, 0x000000, 0x000000, 0x000000, // 2
	 0xffffff, 0xabe7ff, 0xc7d7ff, 0xd7cbff, 0xffc7ff, 0xffc7db, 0xffbfb3, 0xffdbab, 0xffe7a3, 0xe3ffa3, 0xabf3fb, 0xb3ffcf, 0x9ffff3, 0x000000, 0x000000, 0x000000  // 3
}


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
	int tile_counter = 0;

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
				//Get upper palette bits from attribute table
				attr_table_byte = 8*(tile_counter/128) + ((tile_counter%128)%32)%4;
				attr_table_group = ((tile_counter%128)/32)/2 + ((tile_counter%128)%32)/2;

				colour_idx = readVRam(attr_table_base+attr_table_byte);
				if(attr_table_group==0)
					colour_idx << 2;
				else if(attr_table_group>1)
					colour_idx >> 2*attr_table_group;
				
				tile_addr = readVRam(current_addr&0x07FF);
				tile_low = readVRam(pattern_table_start+y);
				tile_high = readVRam(pattern_table_start+y+8);

		

				while(x_pixel<8){   // x fine scroll (pixel level)
					temp = tile_high&0x01;
					colour_idx&=0xC0;
					//temp_black_and_white>>1;
					colour_idx |= (temp<<1 | tile_low&0x01);
					rgb_colour = palette_map[readVRam(BACKGROUND_PALETTE + colour_idx)];
					tile_high>>1;
					tile_low>>1;
					screen_coord_x =  tile + x_pixel;
					SDLBASE::renderPixel(rgb_colour,screen_coord_x,screen_coord_y); //TODO
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
				tile_counter++;

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
	
