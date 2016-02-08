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
		virtual void switch_banks(Memory *mem, int addr);
		virtual void write(Memory *mem, int addr, int value);
			
};

#endif

		
