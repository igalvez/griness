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
	public:
		void switch_banks(Memory *mem, int addr){};
		void write(Memory *mem, int addr, int value){};
			
};

#endif

		
