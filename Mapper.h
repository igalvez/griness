#ifndef MAPPER_H
#define MAPPER_H

class Memory;

class Mapper{
	protected:
		int mapper_id;
		int chrBank[2];
		int prgBank;
		int prgMode;
		int chrMode;
		virtual void switch_banks(uint8 *mem, int address);
		virtual void write(uint8 *mem, int addr, int value);
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

		
