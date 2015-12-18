#include "types.h"


#ifndef MEMORY_H
#define MEMORY_H


class Memory{
	public:
		uint8 *map[0xFFFF];

		Memory();
		int loadGame(string gameName);

}

#endif
