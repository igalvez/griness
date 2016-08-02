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
	start_addr = 0;
}

void PPU::initialize(Memory *mem){
	
	for (int i=0; i<8; i++){
		regs[i] = mem->map[0x2000 + i];
		*regs[i] = 0;
	}
	dmaReg = mem->map[0x4014];
	dmaReg = 0;
	//row=0;
	vblank_counter=0;
	current_addr = start_addr;

	if (!initSDL()){
		printf("Error initializing SDL");
	}
	//*regs[0] = 0x9F;
	
}


uint8 PPU::readVRam(uint16 addr){
	uint16 address = addr;
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
	}
	return VRAM[address];
}

void PPU::writeVRam(uint16 addr, uint8 value){
	uint16 address = addr;
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
	}
	//return VRAM[address];
}

void PPU::executeNMI(){
	x_pixel = 0;
	printf("\n EXECUTE NMI \n");
	*regs[2] = *regs[2]|0x80;
}


void PPU::writeScroll(){
	uint16 temp;
	printf("\nPPU WRITE Scroll\n");
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
	printf("\nPPU WRITE Addr\n");
	temp = *regs[6];
	if(!write_toggle){
	// Write upper byte to current_addr
		current_addr = (temp<<8) | (current_addr&0x00FF); 
		write_toggle = 1;
	}
	else{
		current_addr = temp | (current_addr&0xFF00);
		write_toggle = 0;
	}
	//toggle();
}

void PPU::writeData(){
	printf("\nPPU WRITE DATA \n");
	writeVRam(current_addr,*regs[7]);
	current_addr++;
}

void PPU::readData(){
	printf("\nPPU READ DATA \n");
	*regs[7] = readVRam(current_addr);
	current_addr++;
}


void PPU::doDMA(uint8 **map, uint8 value){
	uint16 addr = value*0x100;
	for(int i =0; i<256; i++){
		
		SPRRAM[i] = *map[addr + i];
		//Aprintf("addr:%x, Data: %x\n",(addr+i),SPRRAM[i]);
	}
	printf("FINISH DMA\n");
}


void PPU::show_pattern_table(){
	printf("name table\n");
	for(int i=0;i<CHR_BANK_SIZE;i++){
		printf("addr: %x, data:%x\n",i,VRAM[i]);
	}

}


//void PPU::clearVBL(){
	

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
	SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0xFF);
	SDL_RenderClear(renderer);
	for(int i=0;i<0x1000;i+=0xF){ //1C2

		printf("ADDR %x = %x\n",i,readVRam(i+addr));
		tile_line = i/((0xF)*0x10);
		tile_col = (i/0xf)%0x10;
		printf("TILE LINE = %d\n",tile_line);
		printf("TILE COL = %d\n",tile_col);	

		for(int line=0;line<8;line++){
			tile_low = readVRam(i+line+addr);
			tile_high = readVRam(i*2+line+addr);
			temp2 = tile_high|tile_low;	
			//printf("tile_low %x\n",tile_low);
			//printf("tile_high %x\n",tile_high);
			for(int col=0; col<8; col++){
				
					temp = tile_high&0x01;
					colour_idx = (temp<<1)|tile_low;
					rgb_colour = palette_map[readVRam(BACKGROUND_PALETTE + colour_idx)];
					printf("pos linha %d, coluna %d\n",tile_line*8+line, tile_col*8+col);
					
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

void PPU::renderBackground(){

	/* VRAM address registers:
        FEDC BA98 7654 3210                  
		.yyy NNYY YYYX XXXX*/
	
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
			//printf("\n");
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
					
					/*if(temp2&0x01==0x01){
						printf("0");
					}
					else{
						printf(" ");
					//}*/
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
}


















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
	
	window = SDL_CreateWindow("SDL2 first window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);

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
