
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
using namespace std;

class Memory{
	public:
		uint8 *mem[8];
		uint8 *RAM;
		uint8 *map[8];

		Memory(){
			create_mem_map();
			uint8 k = 1;
			*mem[0] = 1;
			*mem[1] = 2;
			*mem[2] = 3;
			*mem[3] = 4;
			//print_blocks();

			RAM = new uint8[2];
			RAM[0] = 6;
			RAM[1] = 12;

			int j;
			for (int i=0; i<8; i++){
				j = i%2;
				map[i] = &RAM[j];
			}
			print_blocks2();
		}
		void create_mem_map(){
			uint8 *k;
			k = new uint8[4];
	
			int j=0;
			for (int i=0; i<4; i++){
				mem[i] = new uint8;
			}
			for (int i=4; i<8; i++){
				mem[i] = mem[j];
				j++;
			}

		}
		int read(int addr){
			return *mem[addr];
		}
		int read(int addr, int blocks){
			int aux = 0;
			for(int i=0;i<blocks;i++){
				aux |= (*mem[addr+i] << i*8);
			}
			return aux;
		}

		void write(int addr, int value, int blocks){

			if(blocks<=0 || blocks>4){
				printf( " %d BLOCKS, TOO MANY \n",blocks);
				return;
			}
	
			printf("WILL WRITE %d BLOCKS \n",blocks);
			for(int i=0;i<blocks;i++){
				*mem[addr+i] = (value >> 8*i) & 0x00FF;
			}

		}
		void write(int addr, int value){

			*mem[addr] = value;
			/*int blocks = log2(value)/8;


			int hexv[] = {0x000000FF, 0x0000FF00, 0x00FF0000,0xFF000000};

			if(int(log2(value))%8 > 0){
				blocks+=1;
			}

			printf("NUMBER OF BLOCKS TO WRITE %d\n",blocks);
			
			for(int i=0;i<blocks;i++){
				*mem[addr+i] = (uint8) ((value & hexv[i]) >> i*8);
			}*/
			/*if(value<=0xFF){
				*mem[addr] = value;
			}
			else{
				*mem[addr] = (uint8) (value & 0x00FF);
				*mem[addr+1] = (uint8) ((value & 0xFF00) >> 8);
				printf("ADDR0 %x\n", *mem[addr]);
				printf("ADDR1 %x\n", *mem[addr+1]);
			}*/
		}

		void print_blocks(){
			for(int i=0; i<8; i++){
				printf( "%d - %x \n", i, (int) *mem[i]);
			}
		}
	
		void print_blocks2(){
			for(int i=0; i<8; i++){
				printf( "%d - %d \n", i, (int) *map[i]);
			
			}
			printf("RAM, 1-%d, 2-%d \n", RAM[0], RAM[1]);
		}
};

int main(void){
	Memory m;
	/*int k = 0x5015;
	cout << "THIS K IS " << (int) k << "\n";
	cout << "SMALL TEST m[0]" << *m.mem[0] <<"\n"; 
	m.print_blocks();
	printf( "WILL WRITE k = %x\n", k);
	m.write(0,k,6);
	printf("THE READ IS %x \n\n", m.read(0,2));*/
	m.print_blocks2();
	uint8 k = 27;
	*m.map[3] = k;

	m.print_blocks2();
	//uint8 j = m.read(0);

	//printf(" J IS %x \n\n", j);
	
	
}



