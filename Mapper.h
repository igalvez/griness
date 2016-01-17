#include "Memory.h"

#ifndef MAPPER_H
#define MAPPER_H



class Mapper{
	protected:
		int mapper_id;
		int chrBank[2];
		int prgBank;
		int prgMode;
		int chrMode;
		virtual void change_banks(uint8 *mem, int address);
	public:
		int get_mem_address(uint8 *mem, int address)
		{
			int bank_addr = address/bank_size;
			if (bank_addr != bank_size){			
				change_banks(*mem, bank_addr);
			}
			return address%bank_size;
		}	
			
};

#endif

		
