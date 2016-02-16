#include <stdio.h>
#include <sys/stat.h>


#define GNAME "AirFortress.nes"

typedef unsigned char uint8;

int main(void){

	FILE *game;
	int size;
	unsigned char *gameROM;
	game = fopen(GNAME, "rb");
		
	

	if(!game){
		printf("ERROR OPENING THE FILE \n");
		return 1;
	}
	struct stat st;

    if (stat(GNAME, &st) == 0){
		size = st.st_size;
		printf("FILE SIZE = %d \n", size);
	}
	else{
		printf("Error determining the file size \n");
		return 1;
	}

	gameROM = new uint8[size];

	fread(gameROM, size*sizeof(uint8), 1, game);

	fclose(game);

	int cont = 0;
	for (int i=0; i<0x200; i++){
		printf("%x - %x \n", i-16, gameROM[i]);
		if(gameROM[i+16]!=0xff){
			//break;
		}
		cont++;
	}
	printf("CONT = %d\n",cont);
	return 0;
}	
	
	
