
#include <iostream>
using namespace std;

void create_mem_map(int *m[]);

int main(){

	int *mem[8];
	create_mem_map(mem);
	
	*mem[0] = 1;
	*mem[1] = 2;
	*mem[2] = 3;
	*mem[3] = 4;

	for(int i=0; i<8; i++){
		cout << i << " - " << *mem[i] << "\n";
	}
	*mem[7] = 985;
	for(int i=0; i<8; i++){
		cout << i << " - " << *mem[i] << "\n";
	}
	cout << "1001 % 800 = " << 0x1001%0x800 << "\n";
}


void create_mem_map(int *m[]){
	int *k;
	k = new int[4];
	
	int j=0;
	for (int i=0; i<4; i++){
		m[i] = new int;
	}
	for (int i=4; i<8; i++){
		m[i] = m[j];
		j++;
	}

}
