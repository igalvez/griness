
#ifndef TYPES_H
#define TYPES_H

#define GAME_MEM_REGION
#define ROM_LIMIT 
#define PRG_BANK_SIZE 16*1024
#define CHR_BANK_SIZE 8*1024
#define VRAM_SIZE 16*1024
#define CPU_CLOCK 1200
#define PPU_CLOCK 1200
#define PPU_SIZE 0x10000
#define FRAME_HEIGHT 30
#define FRAME_WIDTH 32
#define BACKGROUND_PALETTE 0X3F00

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;


typedef signed char sint8;
typedef signed short sint16;
typedef signed long sint32;

#endif


