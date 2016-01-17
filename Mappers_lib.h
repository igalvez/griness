#include "Mapper.h"

#ifndef MAPPERS_LIB_H
#define MAPPERS_LIB_H

class MMC1: public Mapper{
	public:
		MMC1(){
			mapper_id = 1;
		}
	private:
		int shift_reg;
		bool prgRam; // enabled or disabled
};

class MMC3: public Mapper{
	public:
		MMC3(){
			mapper_id = 4;
		}
};

class MMC5: public Mapper{
	public:
		MMC5(){
			mapper_id = 5;
		}
};

#endif
