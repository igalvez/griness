#include "types.h"
#include "Memory.h"
#include <stdio.h>

using namespace std;

#ifndef CPU_H
#define CPU_H



class CPU{
	public:
		uint16 pc; // program counter
		uint8 sp;  // stack pointer
		uint8 A;   // accumulator
		uint8 X;  // index register X
		uint8 Y;  // index register Y
		uint8 P;  // Processor status

		//flags
		uint8 N;
		uint8 Z;
		uint8 C;
		uint8 I;
		uint8 D;
		uint8 V;

		Memory *memory;
		uint16 opcode;
		int operand; // Help variable to store operands used by instruction
		int executeOpcode();
		void fetchOpcode();


	//public:
		void initialize(Memory *memP);
		void emulateCycle();
		static int opcodeCycles[256];
		static int opcodeSize[256];
		static int opcodeMode[256];
		
		void compareElements(uint8 reg);
		void store_value(int addr, uint8 value);
		uint8 get_value(int addr);

};

#endif
