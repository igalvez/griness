#include "Mapper.h"


class Cartridge {
	private:
		uint8 *gameROM;
	public:
		Mapper mapper;
		bool loadGame(string gameName){
			char *buffer;
			int size_game;

			ifstream newgame(gameName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

			if (newgame.is_open()){
				size_game = newgame.tellg();
				newgame.seekg(0, ios::beg);
		
				if (size_game <= (4096-512)){
					buffer = new uint8[size_game];
					newgame.read(buffer, size_game);
					for(int i=0; i<size_game; i++){
						map[] = buffer[i];
					}
					return true;
				}
				else{
					cout << "File too big!";
					return false;
				}
			}
			return false;
		}
}
		
