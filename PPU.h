#include "types.h"
//#include "VideoRAM.h"
#include <SDL2/SDL.h>

#ifndef PPU_H
#define PPU_H


class Memory;
class PPU {
	private:
		
		uint8 *dmaReg; // Located on CPU mem at addr $4014
		uint8 *regs[8]; // Addrs $2000-$2007 from CPU memory
		uint16 current_addr; // VRAM rendering current address
		uint16 write_addr;
		long int ppuCycles;

		// Address of top left corner tile (where to start rendering)
		uint16 start_addr; 
		uint8 write_toggle; // First or second write (should use only one bit)

		uint16 ioAddr;
		uint8 ioData;
		uint8 x_pixel;
		uint8 VRAM[VRAM_SIZE];
		uint8 *map[PPU_SIZE];
		uint8 SPRRAM[256];
		uint8 mirroring;
		int row;
		int vblank_counter;
		int screen_coord_x;
		int screen_coord_y;
		static unsigned int palette_map[64];

		/*Internal registers */

		uint16 low_bg, high_bg;
		uint16 init_addr; // Start address of current scanline
		uint8 low_palette, high_palette;
		int scanline, dots;
		
	public:
		//VideoRAM *RAM;
		PPU();
		void initialize(Memory *mem);
		uint8 readVRam(uint16 addr);
		void writeVRam(uint16 addr, uint8 value);
		void executeNMI();
		void renderBackground();

		void doDMA(uint8 **map, uint8 value);

		void writeAddr();
		void writeScroll();
		void writeData();
		void readData();
		void clearVBL();

		void show_pattern_table();
		void showGraphics(uint8 r, uint8 g, uint8 b);
		void showGraphics(uint8 r,uint8 g, uint8 b,int x, int y);
		bool keyboard_Event();
		bool initSDL();
		bool loadMedia();
		void close();
		void paintGraphics();
		void showPatternTable(int addr);
		void showNameTable(uint16 ntb, uint16 ptb);
		void showNameTables();
		void emulateCycle(int cycles);
		void showPixel(int colour_idx, int x, int y);
};


#endif
