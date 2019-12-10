#include "PPU.h"
#include "Memory.h"



#define SCALE 10


SDL_Window *window = NULL;

//surface containing the window
SDL_Surface *screenSurface = NULL;

//Image to be loaded and shown on the screen
SDL_Surface *imageb = NULL;


SDL_Renderer *renderer = NULL;

unsigned int PPU::palette_map[64] ={

    //0      , 1       , 2       , 3       , 4       , 5       , 6       , 7       , 8       , 9       , A       , B       , C       , D       , E       , F
	 0x757575, 0x271b8f, 0x0000ab, 0x47009f, 0xab0073, 0xab0013, 0xa70000, 0x7f0b00, 0x432f00, 0x004700, 0x005100, 0x003f17, 0x1b3f3f, 0x000000, 0x000000, 0x000000, // 0
	 0xbcbcbc, 0x0073ef, 0x233bef, 0x8300f3, 0xbf00bf, 0xe7005b, 0xdb2b00, 0xcb4f0f, 0x8b7300, 0x009700, 0x00ab00, 0x00933b, 0x00838b, 0x000000, 0x000000, 0x000000, // 1
	 0xffffff, 0x3fbfff, 0x5f97ff, 0xa78bfd, 0xf77bff, 0xff77b7, 0xff7763, 0xff9b3b, 0xf3bf3f, 0x83d313, 0x4fdf4b, 0x58f898, 0x00ebdb, 0x000000, 0x000000, 0x000000, // 2
	 0xffffff, 0xabe7ff, 0xc7d7ff, 0xd7cbff, 0xffc7ff, 0xffc7db, 0xffbfb3, 0xffdbab, 0xffe7a3, 0xe3ffa3, 0xabf3fb, 0xb3ffcf, 0x9ffff3, 0x000000, 0x000000, 0x000000  // 3
};


PPU::PPU(){
	//this->RAM = new VideoRAM();
	
	current_addr = 0;
	write_addr = 0;
	start_addr = 0;
}

void PPU::initialize(Memory *mem){
	int i = 0;
	for (i=0; i<8; i++){
		regs[i] = mem->map[0x2000 + i];
		*regs[i] = 0;
	}
	dmaReg = mem->map[0x4014];
	dmaReg = 0;
	//row=0;
	vblank_counter=0;
	write_addr = start_addr;
	init_addr = 0x2000 + (*regs[0]&0x03)*0x400;
	mirroring = mem->cartridge->mirroring;
	dots = 0;
	scanline = 261;

	//scanf("%d",&i);
	/* Mapping VRAM */
	printf("MAPPING VRAM \n");
	for (int i=0; i<0x2000; i++){
		map[i] = &VRAM[i];
	}
	//scanf("%d",&i);
	/* Mirroring 
	   0: Horizontal
	   1: Vertical
	   2: Four-Screen
	  */

	printf("MAPPING MIRRORING = %d \n", mirroring);
	if (!mirroring){
		for (i=0; i<0x400; i++){
			map[0x2000+i] = &VRAM[0x2000 + i];
			map[0x2400+i] = &VRAM[0x2000 + i];
			map[0x2800+i] = &VRAM[0x2400 + i];
			map[0x2C00+i] = &VRAM[0x2400 + i];
			//VRAM[0x2800] = VRAM
		}
	}
	else if(mirroring==1){
		for (i=0; i<0x400; i++){
			map[0x2000+i] = &VRAM[0x2000 + i];
			map[0x2400+i] = &VRAM[0x2400 + i];
			map[0x2800+i] = &VRAM[0x2000 + i];
			map[0x2C00+i] = &VRAM[0x2400 + i];
			//VRAM[0x2800] = VRAM
		}
	}

	printf("MAPPING 3 \n");
	for (i=0;i<0x0F00; i++){
		map[0x3000 + i] = &VRAM[0x2000 + i];
	}
	printf("MAPPING 4\n");
	for (i=0; i<0x20; i++){
		map[0x3F00 + i] = &VRAM[0x2800 + i];
	}
	printf("MAPPING 5 \n");
	for (i=0;i<0xE0;i++){
		map[0x3F20 + i] = map[0x3F00 + i];
	}
	printf("MAPPING 6 \n");
	for (i=0;i<0xC000;i++){
		map[0x4000 + i] = map[i];
	}

	/*if (!initSDL()){
		printf("Error initializing SDL");
	}*/
	//*regs[0] = 0x9F;
	
}


uint8 PPU::readVRam(uint16 addr){
	/*uint16 address = addr;
	if (addr>=0x4000){
		address = addr%0x4000;
	}
	if (address<0x3000){
		//address = address%0x3000
		return VRAM[address];
	}
	else if(address<0x3f00){
		address = address%0x3000;
	}
	else if(address<0x3f20){
		return VRAM[address];
	}
	else if(address<0x4000){
		address = (address%0x3f20) + 0x3f00;
	}
	else{
		address = 0;
	}*/
	return *map[addr];
}

void PPU::writeVRam(uint16 addr, uint8 value){
	/*uint16 address = addr;
	if (addr>=0x4000){
		address = addr%0x4000;
	}
	if (address<0x3000){
		//address = address%0x3000
		VRAM[address] = value;
	}
	else if(address<0x3f00){
		VRAM[address%0x3000] = value;
	}
	else if(address<0x3f20){
		VRAM[address] = value;
	}
	else if(address<0x4000){
		VRAM[(address%0x3f20) + 0x3f00] = value;
	}*/
	//printf("PPU WRITE: writing data %x to address %x\n", value, addr);
	*map[addr] = value;
	//printf("map[%x] = %x\n", addr, value);

	//return VRAM[address];
}

void PPU::executeNMI(){
	x_pixel = 0;
	printf("\n EXECUTE NMI \n");
	*regs[2] = *regs[2]|0x80;
}


void PPU::writeScroll(){
	uint16 temp;
	//printf("\nPPU WRITE Scroll\n");
	temp = *regs[5];
	if(!write_toggle){
	// Write upper byte to start_addr
		start_addr = (temp<<8) | (start_addr&0x00FF); 
		write_toggle = 1;
	}
	else{
		start_addr = temp | (start_addr&0xFF00);
		write_toggle = 0;
	}
	//toggle();
}



void PPU::writeAddr(){
	uint16 temp;
	//printf("\nPPU WRITE Addr\n");
	temp = *regs[6];
	if(!write_toggle){
	// Write upper byte to write_addr
		write_addr = (temp<<8) | (write_addr&0x00FF); 
		write_toggle = 1;
	}
	else{
		write_addr = temp | (write_addr&0xFF00);
		write_toggle = 0;
	}
	//printf("addr %x\n", write_addr);
	//toggle();
}

void PPU::writeData(){
	//printf("\nPPU WRITE DATA \n");
	//printf("addr %x\n", write_addr);
	//uint16 addr = (write_addr&0x0FFF) + 0x2000;
	//printf("addr %x\n", addr);
	writeVRam(write_addr,*regs[7]);
	//write_addr++; TODO: ON NON DEBUG CODE UNCOMMENT THIS!!!
}

void PPU::readData(){
	//printf("\nPPU READ DATA \n");
	*regs[7] = readVRam(write_addr);
	uint8 inc32 = (*regs[0]&0x04)>>2;
	if (inc32){
		write_addr += 32;
	}
	else {
		write_addr++;
	}
}


void PPU::doDMA(uint8 **map, uint8 value){
	uint16 addr = value*0x100;
	for(int i =0; i<256; i++){
		
		SPRRAM[i] = *map[addr + i];
		//Aprintf("addr:%x, Data: %x\n",(addr+i),SPRRAM[i]);
	}
	//printf("FINISH DMA\n");
}


void PPU::show_pattern_table(){
	//printf("name table\n");
	for(int i=0;i<CHR_BANK_SIZE;i++){
		//printf("addr: %x, data:%x\n",i,VRAM[i]);
	}

}


//void PPU::clearVBL(){
	
/*void PPU::emulateCycle(){

}*/


void PPU::renderBackground(){
	//uint16 tile_addr;
	uint8 tile_low;
	uint8 tile_high;
	//uint8 temp;
	uint16 pattern_table_start = 0x0000;
	uint16 attr_table_base = (write_addr&0x0FFF) + 0x2800 + 0x3C0;
	

	uint8 attr_table_byte;
	uint8 attr_table_group;
	uint8 colour_idx;
	uint8 rgb_colour;
	uint8 tile_idx[32];
	uint16 aux;

	int tile_counter = 0;

	uint16 temp2;	

	

	/*if(vblank_counter>0){
		if(vblank_counter<=20){
			//printf("scanline %d\n",239+vblank_counter);
			vblank_counter++;
				
			return;
		}
		vblank_counter=0;
		write_addr = start_addr;
		screen_coord_y = 0;
		//printf("BEGIN FRAME \n");
		//SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0xFF);
		//SDL_RenderClear(renderer);
	}	*/
	
	/* VRAM address registers:
       FEDC BA98 7654 3210                  
	   .yyy NNYY YYYX XXXX 
    */
 	uint8 y = (write_addr&0x7000)>>12;

	if(y==0){
		
		for(int col=0;col<32;col++){
			//printf("SCANLINE 1 of tile %d\n",col);
			 //readVRam(addr)*16
			aux = (write_addr&0x0FFF) + 0x2000;
			//printf("NAMETABLE ADDR = %x\n",aux);
			//printf("prefix addr =%x\n",current_addr&0x0FFF);
			tile_idx[col] = readVRam(aux)*16;
			screen_coord_x = 8*col;
			tile_low = readVRam(pattern_table_start+tile_idx[col]);
			tile_high = readVRam(pattern_table_start+tile_idx[col]+8);
			temp2 = tile_low|tile_high;
			//printf("PATTERN IDX %d\n",tile_idx[col]);
			//printf("tile low %x\n",tile_low);
			//printf("tile high %x\n",tile_high);
			while(x_pixel<8){ 
				
				//printf("screen col %d\n",screen_coord_x);
				//printf("screen line %d\n",screen_coord_y);
				/*if((temp2&0x80)==0x80){
  					showGraphics(0x00, 0x00, 0x00, screen_coord_x, screen_coord_y);
				}
				else{
					showGraphics(0xff, 0xff, 0xff, screen_coord_x, screen_coord_y);
				}//*/
				temp2 = temp2<<1;
				x_pixel++;
				screen_coord_x++;
			}
			x_pixel=0;
			if ((write_addr&0x001F)==0x001F){ // If reached end of nametable, clean x scroll and switch bit 10 (nametable)
				write_addr&=0xFFE0; 
				write_addr = (~write_addr&0x0400) | (write_addr&0xFBFF);
				//attr_table_base= write_addr&0x0FFF + 0x2000 + 0x3C0;
			}
			else{
				write_addr++;
			}
		}
	}
	else{
		
		for(int col=0;col<32;col++){
			//printf("SCANLINE %d of tile %d\n",y,col);
			tile_low = readVRam(pattern_table_start+tile_idx[col]+y);
			tile_high = readVRam(pattern_table_start+tile_idx[col]+y+8);
			temp2 = tile_low|tile_high;	
			screen_coord_x = 8*col;

			while(x_pixel<8){ 
				//printf("screen col %d\n",screen_coord_x);
				//printf("screen line %d\n",screen_coord_y);
				/*if((temp2&0x80)==0x80){
  					showGraphics(0x00, 0x00, 0x00, screen_coord_x, screen_coord_y);
				}
				else{
					showGraphics(0xff, 0xff, 0xff, screen_coord_x, screen_coord_y);
				}//*/
				temp2 = temp2<<1;
				x_pixel++;
				screen_coord_x++;
			}
			x_pixel=0;
			if ((write_addr&0x001F)==0x001F){ // If reached end of nametable, clean x scroll and switch bit 10 (nametable)
				write_addr&=0xFFE0; 
				write_addr = (~write_addr&0x0400) | (write_addr&0xFBFF);
				//attr_table_base= write_addr&0x0FFF + 0x2000 + 0x3C0;
			}
			else{
				write_addr++;
			}
		}
	}	

	//Set x back to initial coordinates so we can start at the beginning of next scanline		
	write_addr = (start_addr&0x0C1F) | (write_addr&0xF3E0); 
	write_addr += 0x1000;	

	if((y+1)>7){
		write_addr &= 0x0FFF;
		if((write_addr&0x03E0)==0x03E0){
		 // End of nametable
		 // Reset Y scroll and switch bit 11 (nametable)
			write_addr&=0xFC1F; 
			write_addr = (~write_addr&0x0800) | (write_addr&0xF7FF);
		}
		else{
			write_addr+=0x0020;
		}
	}
	if(screen_coord_y>239){
		row=0;
		executeNMI();
		//vblank_counter=1;

		printf("RENDER \n");
	    screen_coord_y = 0;	
		
	}
	SDL_RenderPresent(renderer);
	screen_coord_y++;
}

/*void PPU::renderBackground(){

	/* VRAM address registers:
        FEDC BA98 7654 3210                  
		.yyy NNYY YYYX XXXX
	
	uint16 tile_addr;
	uint8 tile_low;
	uint8 tile_high;
	uint8 temp;
	uint16 pattern_table_start = 0x3F00;
	uint16 attr_table_base = current_addr&0x0FFF + 0x2000 + 0x3C0;
	

	uint8 attr_table_byte;
	uint8 attr_table_group;
	uint8 colour_idx;
	uint8 rgb_colour;

	int tile_counter = 0;

	uint16 temp2;

	//uint16 ppuaddr = 0x2000 + (start_addr&0x03FF);
	

	
	//uint16 temp;
	printf("\n\n\nRENDER BACKGROUND\n\n\n");

	printf("\n Current y addr %x \n",current_addr);
	printf("\n ROW %x \n\n",row);
	//show_pattern_table();
//	for(int row=0; row<FRAME_HEIGHT; row++)
	//while(row<FRAME_HEIGHT){
		//nametable_addr = current_addr&0x07FF;
		
		if(vblank_counter>0){
			if(vblank_counter<=20){
				printf("scanline %d\n",239+vblank_counter);
				vblank_counter++;
				
				return;
			}
			vblank_counter=0;
			current_addr = start_addr;
		}
		//while((current_addr&0xF000)<=0x7000){  // y fine scroll corresponding to scanline
		//for(int scanline=0; scanline<8; scanline++){//8 scanlines per tile row
			uint8 y = (current_addr&0x7000)>>12; // pixel line of tile
			screen_coord_y = row*8 + y;
			printf("scanline %d\n",screen_coord_y);
			for(int tile=0; tile<FRAME_WIDTH; tile++){
				//Get upper palette bits from attribute table
				attr_table_byte = 8*(tile_counter/128) + ((tile_counter%128)%32)%4;
				attr_table_group = ((tile_counter%128)/32)/2 + ((tile_counter%128)%32)/2;
				
				if(y==0){
					//printf("0");
				}
				colour_idx = readVRam(attr_table_base+attr_table_byte);
				if(attr_table_group==0)
					colour_idx << 2;
				else if(attr_table_group>1)
					colour_idx >> 2*attr_table_group;
				
				tile_addr = readVRam(current_addr&0x0FFF + 0x2000);
				tile_low = readVRam(pattern_table_start+y);
				tile_high = readVRam(pattern_table_start+y+8);
				temp2 = tile_high|tile_low;
		
				printf(" ");
				while(x_pixel<8){   // x fine scroll (pixel level)
					temp = tile_high&0x01;
					colour_idx&=0xC0;
					//temp_black_and_white>>1;
					colour_idx |= (temp<<1 | tile_low&0x01);
					rgb_colour = palette_map[readVRam(BACKGROUND_PALETTE + colour_idx)];
					
					/*if(temp2&0x01==0x01){
						printf("0");
					}
					else{
						printf(" ");
					}
					tile_high = tile_high>>1;
					tile_low >>= 1;
					temp2>>=1;
					screen_coord_x =  tile*8 + x_pixel;
					//SDLBASE::renderPixel(rgb_colour,screen_coord_x,screen_coord_y); //TODO
					uint8 red = (rgb_colour&0x00FF0000)>>16;
					uint8 green = (rgb_colour&0x0000FF00)>>8;
					uint8 blue = rgb_colour&0x000000FF;
					showGraphics(red, green, blue);
					x_pixel++;
					//printf(".");
				}

				x_pixel = 0;
				if (current_addr&0x001F==0x001F){ // If reached end of nametable, clean x scroll and switch bit 10 (nametable)
					current_addr&=0xFFE0; 
					current_addr = (~current_addr&0x0400) | current_addr&0xFBFF;
					attr_table_base= current_addr&0x0FFF + 0x2000 + 0x3C0;
				}
				else{
					current_addr++;
				}
				tile_counter++;

			}

			
			//Set x back to initial coordinates so we can start at the beginning of next scanline
			current_addr = (start_addr&0x001F) | (current_addr&0xFFE0); 
			current_addr = (~current_addr&0x0400) | current_addr&0xFBFF; //switch bit 10 to go back to initial nametable
			attr_table_base = current_addr&0x0FFF + 0x2000 + 0x3C0;
			current_addr += 0x1000; // Increment y fine scroll (scanline)
			
	

			
		//}
		// Reset y fine scroll (new tile)
		if((current_addr&0xF000)>0x7000){
			current_addr&=0x0FFF;
			if(current_addr&0x03E0==0x03E0){ // If last row of nametable
				// Reset Y scroll and switch bit 11 (nametable)
				current_addr&=0xFC1F; 
				current_addr = (~current_addr&0x0800) | current_addr&0xF7FF;
				//row=0;
				//executeNMI();
				//vblank_counter=1;
				
			}	
			else{
				current_addr+=0x0020;
				//row++;
			}
			row++;
		}
		
		if(screen_coord_y>239){
			row=0;
			executeNMI();
			vblank_counter=1;
			printf("RENDER \n");
			SDL_RenderPresent(renderer);
		}

		
		
	//SDL_RenderPresent(renderer);
	//}

	//executeNMI();
}*/

void PPU::showPixel(int colour_idx, int x, int y){
	int rgb_colour;
	int red, green, blue;
	rgb_colour = palette_map[colour_idx];

	red = (rgb_colour&0x00FF0000)>>16;
	green = (rgb_colour&0x0000FF00)>>8;
	blue = rgb_colour&0x000000FF;
	showGraphics(red, green, blue, x, y);

}

void PPU::emulateCycle(int cycles){ 
	
	uint16 ntable_addr = 0x2000 + (*regs[0]&0x03)*0x400;
	uint16 ptable_addr = 0x0000 + ((*regs[0]&0x10)>>4)*0x1000;
	uint16 pindex; // pattern table index
	uint8 inc = 1;
	//uint16 init_addr = ptable_addr;
	uint8 colour_idx = 0;
	uint8 pbyte = 0;

	if (*regs[0]&0x04==0x04){
		inc = 32;
	}

	for (int i=0; i<cycles; i++){

		//printf("scanline: %d, dots: %d, PPU addr = %x\n", scanline, dots, current_addr);
		/* Visible scanlines */
		if (scanline<240){
			/* Every 8 cycles new tile data is loaded into shift register*/
			if ((dots<256) && ((dots+1)%8==0)){
				/* Getting background data from pattern tible (high and low bytes of tile data) */
				pindex = readVRam(current_addr);
				low_bg = (low_bg | readVRam(ptable_addr + pindex*16));
				high_bg = (high_bg | readVRam(ptable_addr + pindex*16 + 8));
				current_addr += inc; 
			}
			else if (dots<256){
				colour_idx = (high_bg>>14) | (low_bg>>15);
				high_bg <<= 1;
				low_bg <<= 1;
				showPixel(colour_idx, dots, scanline);
			}
			else if (dots==256){
				if ((scanline+1)%8==0){
					init_addr = ntable_addr + ((scanline+1)/8)*32;
				}
				current_addr = init_addr;
				pindex = readVRam(current_addr);
				low_bg = readVRam(ptable_addr + pindex*16);
				high_bg = readVRam(ptable_addr + pindex*16 + 8);
				low_bg <<= 8;
				high_bg <<= 8;
				current_addr += inc;


			}
			else if (dots==257){
				pindex = readVRam(current_addr);
				low_bg = low_bg | readVRam(ptable_addr + pindex*16);
				high_bg = high_bg | readVRam(ptable_addr + pindex*16 + 8);
				current_addr += inc;
			}

		}
		else if (scanline<261) { //VBLANK

			if ((scanline == 241) && (dots==1)){
				//SET VBLANK
				*regs[2] = *regs[2] | 0x80;
				init_addr = ntable_addr;
			}
		}
		else {
			if (dots==1){
				// Clear VBLANK FLAG
				*regs[2] = *regs[2]&0x7F;

				/*if (scanline%8 == 0){
					init_addr = ntable_addr + (scanline/8)*32;
					pbyte =  (scanline+1)%8;
					current_addr = init_addr;
				}*/

			}
			else if (dots==2){
				pindex = readVRam(current_addr);
				low_bg = readVRam(ptable_addr + pindex*16);
				high_bg = readVRam(ptable_addr + pindex*16 + 8);
				low_bg <<= 8;
				high_bg <<= 8;
				current_addr += inc;
			}
			else if (dots==3){
				pindex = readVRam(current_addr);
				low_bg = low_bg | readVRam(ptable_addr + pindex*16);
				high_bg = high_bg | readVRam(ptable_addr + pindex*16 + 8);
				current_addr += inc;
			}
			//dots = 0;
		}
		dots++;

		if (dots>340){
			dots = 0;
			scanline++;
		}
		if (scanline>261){
			scanline = 0;
			//init_addr = ntable_addr;
		}


	}
	//printf("%d PPU cycles emulated\n", cycles);
	//printf("scanline = %d, dot = %d\n",scanline, dots);
	SDL_RenderPresent(renderer);

}

void PPU::showNameTables(){
	uint16 ntables_addr[4] = {0x2000, 0x2400, 0x2800, 0x2C00};
	uint16 ntable_size = 0x03C0;
	int line, col, pixline, pixcol;
	uint8 tile_idx, colour_idx, tile_addr;
	uint16 ptable;
	int rgb_colour[3];
	int count = 0;
	uint8 lower_data, higher_data;
	uint8 temp;
	int x =0;
	int y = 0;

	SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0xFF);
	SDL_RenderClear(renderer);
	ptable = (0x0000 | ((*regs[0]&0x10)>>1));
    //ptable=0x1000;
	printf("PTABLE= %x\n", ptable);
	int addr = 0x2C00;
	//for (int addr=0x2000; addr<0x2FC0; addr+=0x400){
		//printf("table addr= %x\n", addr);
		for (line=0; line<30; line++){
			printf("\n%4x> ", addr+line*32);
			for (col=0;col<32;col++){

				tile_idx = readVRam(addr+col+line*32);   
				tile_addr = tile_idx*0xF;
				printf("%x ", tile_idx);

				for (pixline=0; pixline<8; pixline++){
					lower_data = readVRam(tile_addr + pixline + ptable);
					higher_data = readVRam(tile_addr + pixline + 0x8 + ptable);
					//colour_idx = ((higher_data&0x80)>>6)|((lower_data&0x80)>>7);
					for (pixcol=0; pixcol<8; pixcol++){
						//colour_idx = (tile_high&0x80)>>6;
						colour_idx = ((higher_data&0x80)>>6)|((lower_data&0x80)>>7);
						//printf("coulour_idx = %u, rgb_colour = %x\n",colour_idx, palette_map[colour_idx]);
						rgb_colour[0] = (palette_map[colour_idx]&0x00FF0000)>>16;
						rgb_colour[1] = (palette_map[colour_idx]&0x0000FF00)>>8;
						rgb_colour[2] = (palette_map[colour_idx]&0x000000FF);
						showGraphics(rgb_colour[0], rgb_colour[1], rgb_colour[2], x + col*8+pixcol, y+line*8+pixline);
						lower_data <<= 1;
						higher_data <<= 1;

					}
				}
			}
		}
		y = 256*(count/2);
		x = 240*(count%2);
		count++;
	////}
	executeNMI();
	SDL_RenderPresent(renderer);

}

void PPU::showNameTable(uint16 ntb, uint16 ptb){

	uint16 addr = ntb;
	uint8 tile_idx, tile_low, tile_high;
	uint8 temp2;
	uint16 aux,naddr;
	for(int line=0;line<30;line++){
		for(int col=0;col<32;col++){
			tile_idx = readVRam(addr);
			naddr = tile_idx*16;
			//printf("TILE IDX = %d\n",tile_idx);
			//printf("ADDR NTB %x\n",naddr);
			//tile_idx = addr;
			for(int pline=0;pline<8;pline++){
				//printf("ptb + tile_idx + pline = %x + %x + %x = %x\n",ptb,tile_idx,pline,(ptb+tile_idx+pline));
				tile_low = readVRam(ptb+naddr+pline);
				tile_high = readVRam(ptb+naddr+pline+8);
				temp2 = tile_high|tile_low;	
				for(int pcol=0;pcol<8;pcol++){
					//printf("npos linha %d, coluna %d\n",line*8+pline, col*8+pcol);
					
					tile_high = tile_high>>1;
					tile_low >>= 1;	
					
					if((temp2&0x80)==0x80){
  						showGraphics(0x00, 0x00, 0x00, col*8+pcol, line*8+pline);
					}
					else{
						showGraphics(0xff, 0xff, 0xff, col*8+pcol, line*8+pline);
					}
					temp2 = temp2<<1;
				}
				//tile_idx++;
	        }	
			addr++;
		}
	}
	SDL_RenderPresent(renderer);
}
				

void PPU::showPatternTable(int addr){
	uint8 tile_low, tile_high;
	uint8 temp2;
	int tile_line = 0;
	int tile_col = 0;
	int rgb_colour;
	uint8 temp;
	uint8 colour_idx;
	uint8 red;
	uint8 green;
	uint8 blue;
	uint16 addr_low, addr_high;
	uint8 tile_matrix[8][8];
	SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0xFF);
	SDL_RenderClear(renderer);
	//printf("0 MARK Is %x\n",readVRam(0x1000));
	printf("IN SHOW PATTERN TABLE \n");
	for(int i=0;i<=0x1000;i+=0x10){ //1C2
 
 		/*for (int line=0; line<8; line++){
 			addr_low = i+line+addr;
			addr_high = i+line+addr+8;
			tile_low = readVRam(addr_low);
			tile_high = readVRam(addr_high);
			temp2 = (tile_high<<1) | tile_low
 			
 		}*/
    
		//printf("ADDR %x = %x\n",i,readVRam(i+addr));
		tile_line = i/((0x10)*0x10);
		tile_col = (i/0x10)%0x10;
		//printf("TILE LINE = %d\n",tile_line);
		//printf("TILE COL = %d\n",tile_col);	
		//printf("\n%x > ",i+addr);

		for(int line=0;line<8;line++){
			//printf("%x ", readVRam(i+line+addr));
			addr_low = i+line+addr;
			addr_high = i+line+addr+8;
			tile_low = readVRam(i+line+addr);
			tile_high = readVRam(i+line+addr+8);
			//temp2 = (tile_high<<1)|tile_low;	
			/*printf("addr low  %x\n",addr_low);
			printf("addr high %x\n",addr_high);
			printf("tile_low %x\n",tile_low);
			printf("tile_high %x\n",tile_high);
			printf("temp2 %x\n",temp2);*/
			for(int col=0; col<8; col++){
				

					temp = (tile_high&0x80)>>6;
					colour_idx = temp | ((tile_low&0x80)>>7);
					//temp = tile_high&0x01;
					//colour_idx = (temp<<1)|(tile_low&0x01);

					temp2 = (colour_idx );// | 0x8);//readVRam(BACKGROUND_PALETTE));
					rgb_colour = palette_map[temp2];//readVRam(BACKGROUND_PALETTE + colour_idx)];
					//rgb_colour = colour_idx<<8 | 2;

					//printf("pos linha %d, coluna %d\n",tile_line*8+line, tile_col*8+col);
					
					tile_high = tile_high<<1;
					tile_low <<= 1;	
					
					red = (rgb_colour&0x00FF0000)>>16;
					green = (rgb_colour&0x0000FF00)>>8;
					blue = rgb_colour&0x000000FF;

					showGraphics(red, green, blue, tile_col*8+col, tile_line*8+line);


					//printf("coulour_idx = %u, temp2 = %x, rgb_colour = %x\n",colour_idx, temp2, rgb_colour);

					/*if (colour_idx==0){
						showGraphics(0x00, 0x00, 0x00, tile_col*8+col, tile_line*8+line);
					}
					else if (colour_idx==1){
						showGraphics(0xff, 0xff, 0xff, tile_col*8+col, tile_line*8+line);
					}
					else {
						showGraphics(75, 0, 130, tile_col*8+col, tile_line*8+line);
					}*/
					//showGraphics(red, green, blue, tile_col*8+col, tile_line*8+line);
					/*if((temp2&0x80)==0x80){
  						showGraphics(0x00, 0x00, 0x00, tile_col*8+col, tile_line*8+line);
					}
					else{
						showGraphics(0xff, 0xff, 0xff, tile_col*8+col, tile_line*8+line);
					}
					temp2 = temp2<<1;*/
	        }	
	        /*for(int line=0x8;line<0x10;line++){
				printf("%x ", readVRam(i+line+addr));
	        }*/
		}		
	}
	SDL_RenderPresent(renderer);
	printf("LEAVING SHOW PATTERN TABLE \n");
}

		
/*		for(int line=0;line<8; line++){
			for(int col=0; col<8; col++){
				
					temp = tile_high&0x01;
					colour_idx = (temp<<1)|tile_low;
					rgb_colour = palette_map[readVRam(BACKGROUND_PALETTE + colour_idx)];
					printf("pos linha %d, coluna %d\n",tile_line*8+line, tile_col*8+col);
					
					if(temp2&0x01==0x01){
						printf("0");
					}
					else{
						printf(" ");
					//}
					tile_high = tile_high>>1;
					tile_low >>= 1;	
					
					red = (rgb_colour&0x00FF0000)>>16;
					green = (rgb_colour&0x0000FF00)>>8;
					blue = rgb_colour&0x000000FF;
					if((temp2&0x01)==1){
  						showGraphics(0x00, 0x00, 0x00, tile_col*8+col, tile_line*8+line);
					}
					else{
						showGraphics(0xff, 0xff, 0xff, tile_col*8+col, tile_line*8+line);
					}
					temp2 = temp2>>1;
	        }
		}
	}

		SDL_RenderPresent(renderer);
}*/


void PPU::showGraphics(uint8 r,uint8 g, uint8 b,int x, int y){
	//SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0xFF);
	//SDL_RenderClear(renderer);
	//printf("linha: %d\n",screen_coord_y);
	//printf("coluna: %d\n",screen_coord_x);

	SDL_Rect pixel = {x*4,y*4,1*4,1*4};
	SDL_SetRenderDrawColor(renderer, r,g,b, 0xFF);
	SDL_RenderFillRect(renderer, &pixel);
}

/*void PPU::renderBackground(){

	/* VRAM address registers:
        FEDC BA98 7654 3210                  
		.yyy NNYY YYYX XXXX
	
	uint16 tile_addr;
	uint8 tile_low;
	uint8 tile_high;
	uint8 temp;
	uint16 pattern_table_start = 0x3F00;
	uint16 attr_table_base = current_addr&0x0FFF + 0x2000 + 0x3C0;
	

	uint8 attr_table_byte;
	uint8 attr_table_group;
	uint8 colour_idx;
	int rgb_colour;
	int tile_counter = 0;

	uint16 temp2;

	//uint16 ppuaddr = 0x2000 + (start_addr&0x03FF);
	current_addr = start_addr;

	
	//uint16 temp;
	printf("\n\n\nRENDER BACKGROUND\n\n\n");
	//show_pattern_table();
	SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0xFF);
	SDL_RenderClear(renderer);
	for(int row=0; row<FRAME_HEIGHT; row++){
		//nametable_addr = current_addr&0x07FF;
		
		while((current_addr&0xF000)<=0x7000){  // y fine scroll corresponding to scanline
		//for(int scanline=0; scanline<8; scanline++){//8 scanlines per tile row
			uint8 y = (current_addr&0x7000)>>12; // pixel line of tile
			screen_coord_y = 8*row + y;
			//printf("\n";
			for(int tile=0; tile<FRAME_WIDTH; tile++){
				//Get upper palette bits from attribute table
				attr_table_byte = 8*(tile_counter/128) + ((tile_counter%128)%32)%4;
				attr_table_group = ((tile_counter%128)/32)/2 + ((tile_counter%128)%32)/2;
				
				
				colour_idx = readVRam(attr_table_base+attr_table_byte);
				if(attr_table_group==0)
					colour_idx << 2;
				else if(attr_table_group>1)
					colour_idx >> 2*attr_table_group;
				
	
				tile_addr = readVRam(current_addr&0x0FFF + 0x2000);
				tile_low = readVRam(pattern_table_start+y);
				tile_high = readVRam(pattern_table_start+y+8);
				temp2 = tile_high|tile_low;
		
				//printf(" ");
				while(x_pixel<8){   // x fine scroll (pixel level)
					temp = tile_high&0x01;
					colour_idx&=0xC0;
					//temp_black_and_white>>1;
					colour_idx |= (temp<<1 | tile_low&0x01);
					rgb_colour = palette_map[readVRam(BACKGROUND_PALETTE + colour_idx)];
					
					if(temp2&0x01==0x01){
						printf("0");
					}
					else{
						printf(" ");
					//}
					tile_high = tile_high>>1;
					tile_low >>= 1;
					temp2>>=1;
					screen_coord_x =  8*tile + x_pixel;
					//SDLBASE::renderPixel(rgb_colour,screen_coord_x,screen_coord_y); //TODO
					//printf("RGB COLOUR %d\n",rgb_colour);
					uint8 red = (rgb_colour&0x00FF0000)>>16;
					uint8 green = (rgb_colour&0x0000FF00)>>8;
					uint8 blue = rgb_colour&0x000000FF;
					//showGraphics(red, green, blue);
					//printf("linha: %d\n",screen_coord_x);
					//printf("coluna: %d\n",screen_coord_y);
					x_pixel++;
					//printf(".");
				}

				x_pixel = 0;
				if (current_addr&0x001F==0x001F){ // If reached end of nametable, clean x scroll and switch bit 10 (nametable)
					current_addr&=0xFFE0; 
					current_addr = (~current_addr&0x0400) | current_addr&0xFBFF;
					attr_table_base= current_addr&0x0FFF + 0x2000 + 0x3C0;
				}
				else{
					current_addr++;
				}
				tile_counter++;

			}
			//Set x back to initial coordinates so we can start at the beginning of next scanline
			current_addr = (start_addr&0x001F) | (current_addr&0xFFE0); 
			current_addr = (~current_addr&0x0400) | current_addr&0xFBFF; //switch bit 10 to go back to initial nametable
			attr_table_base = current_addr&0x0FFF + 0x2000 + 0x3C0;
			current_addr += 0x1000; // Increment y fine scroll (scanline)
	

			
		}
		// Reset y fine scroll (new tile)
		current_addr&=0x0FFF;
		if(current_addr&0x03E0==0x03E0){ // If last row of nametable
			// Reset Y scroll and switch bit 11 (nametable)
			current_addr&=0xFC1F; 
			current_addr = (~current_addr&0x0800) | current_addr&0xF7FF;
				
		}
		else{
			current_addr+=0x0020;
		}

	}


	//SDL_RenderPresent(renderer);
	executeNMI();
}*/


















void PPU::showGraphics(uint8 r,uint8 g, uint8 b){
	SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0xFF);
	SDL_RenderClear(renderer);
	//printf("linha: %d\n",screen_coord_y);
	//printf("coluna: %d\n",screen_coord_x);

	SDL_Rect pixel = {screen_coord_x,screen_coord_y,1,1};
	SDL_SetRenderDrawColor(renderer, r,g,b, 0xFF);
	SDL_RenderFillRect(renderer, &pixel);

	//SDL_Rect outlineRect = { 0,0,200,200}; 
	//SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF ); 
	//SDL_RenderDrawRect( renderer, &outlineRect );
	/*for (int line=0; line<32; line++){
		for (int col=0; col<64; col++){
			if(chip8.gfx[line*64 + col] == 1){
				//printf("BLACK \n");
				SDL_Rect pixel = {col*SCALE,line*SCALE,SCALE,SCALE};
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &pixel);
				//SDL_RenderPresent(renderer);
			}
		}
	}*/

	//SDL_RenderPresent(renderer);
}

/*bool PPU::keyboard_Event(){
	SDL_Event event;
	bool quit = false;

	while(SDL_PollEvent(&event) !=0){
		if (event.type == SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				case SDLK_1:
					chip8.key[0x1] = 1;
					break;
				case SDLK_2:
					chip8.key[0x2] = 1;
					break;
				case SDLK_3:
					chip8.key[0x3] = 1;
					break;
				case SDLK_4:
					chip8.key[0xC] = 1;
					break;
				case SDLK_q:
					chip8.key[0x4] = 1;
					break;
				case SDLK_w:
					chip8.key[0x5] = 1;
					break;
				case SDLK_e:
					chip8.key[0x6] = 1;
					break;
				case SDLK_r:
					chip8.key[0xD] = 1;
					break;
				case SDLK_a:
					chip8.key[0x7] = 1;
					break;
				case SDLK_s:
					chip8.key[0x8] = 1;
					break;
				case SDLK_d:
					chip8.key[0x9] = 1;
					break;
				case SDLK_f:
					chip8.key[0xE] = 1;
					break;
				case SDLK_z:
					chip8.key[0xA] = 1;
					break;
				case SDLK_x:
					chip8.key[0x0] = 1;
					break;
				case SDLK_c:
					chip8.key[0xB] = 1;
					break;
				case SDLK_v:
					chip8.key[0xF] = 1;
			}
		}
		else if (event.type == SDL_KEYUP){
			switch(event.key.keysym.sym){
				case SDLK_1:
					chip8.key[0x1] = 0;
					break;
				case SDLK_2:
					chip8.key[0x2] = 0;
					break;
				case SDLK_3:
					chip8.key[0x3] = 0;
					break;
				case SDLK_4:
					chip8.key[0xC] = 0;
					break;
				case SDLK_q:
					chip8.key[0x4] = 0;
					break;
				case SDLK_w:
					chip8.key[0x5] = 0;
					break;
				case SDLK_e:
					chip8.key[0x6] = 0;
					break;
				case SDLK_r:
					chip8.key[0xD] = 0;
					break;
				case SDLK_a:
					chip8.key[0x7] = 0;
					break;
				case SDLK_s:
					chip8.key[0x8] = 0;
					break;
				case SDLK_d:
					chip8.key[0x9] = 0;
					break;
				case SDLK_f:
					chip8.key[0xE] = 0;
					break;
				case SDLK_z:
					chip8.key[0xA] = 0;
					break;
				case SDLK_x:
					chip8.key[0x0] = 0;
					break;
				case SDLK_c:
					chip8.key[0xB] = 0;
					break;
				case SDLK_v:
					chip8.key[0xF] = 0;
			}
		}
		else if(event.type == SDL_QUIT){
			quit = true;
		}
	}
	return quit;
}
*/


bool PPU::initSDL(){
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		printf("Could not initialize... SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	window = SDL_CreateWindow("SDL2 first window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 960, SDL_WINDOW_SHOWN);

	if(window==NULL){
		printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}
		
			// Get window surface
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		printf("Renderer could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	//screenSurface = SDL_GetWindowSurface(window);
	return true;
}
bool PPU::loadMedia(){
	imageb = SDL_LoadBMP("/data/Documents/chip8/SDL/phd011113s.bmp");
	if(imageb==NULL){
		printf("Error loading image: SDL_Error: %s\n", SDL_GetError());
		return false;
	}	return true;

}

void PPU::close(){
	SDL_FreeSurface(imageb);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



void PPU::paintGraphics(){
	SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0xFF);
	SDL_RenderClear(renderer);


	//SDL_Rect outlineRect = { 0,0,200,200}; 
	//SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF ); 
	//SDL_RenderDrawRect( renderer, &outlineRect );
	for (int line=0; line<32; line++){
		for (int col=0; col<64; col++){
			//if(chip8.gfx[line*64 + col] == 1){
				//printf("BLACK \n");
				SDL_Rect pixel = {col*SCALE,line*SCALE,SCALE,SCALE};
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &pixel);
				//SDL_RenderPresent(renderer);
			//}
		}
	}
	SDL_RenderPresent(renderer);

}
