
#include <iostream>
#include <stdio.h>
using namespace std;

void create_mem_map(int *m[], int matrix[]);
void copy_block(unsigned char *block);



int main(){

	int *mem[8];
	int mat[4] = {20,21,22,23};
	create_mem_map(mem,mat);
	
	*mem[0] = 1;
	*mem[1] = 2;
	*mem[2] = 3;
	*mem[3] = 4;

	for(int i=0; i<8; i++){
		cout << i << " - " << *mem[i] << "\n";
	}
	*mem[7] = 985;
	mat[0] = 128;
	for(int i=0; i<8; i++){
		cout << i << " - " << *mem[i] << "\n";
	}
	cout << "1001 % 800 = " << 0x1001%0x800 << "\n";


	unsigned char *block2;
	copy_block(block2);
	
	

	for (int i =0;i<4;i++){
		cout << "block " << i << " = "<< (int)(char)block2[i] << "\n";
	}

	
}



void copy_block(unsigned char *block){
	char *teste = new char[4];
	for (int i =0;i<4;i++){
		teste[i] = 7 + i;
	}
	//block = new unsigned char[4];
	block = teste;
}

void create_mem_map(int *m[], int matrix[]){
	int *k;

	
	k = new int[4];
	
	int j=0;
	for (int i=0; i<4; i++){
		m[i] = new int;
	}
	for (int i=0; i<4; i++){
		m[i+4] = &matrix[i];
		j++;
	}

}
