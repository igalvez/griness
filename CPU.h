
#include "utils.h"


#ifndef CPU_H
#define CPU_H


int add(Calculator *calc);
int subtract(Calculator *calc);
int multiply(Calculator *calc);
int divide(Calculator *calc);


Class CPU{
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

		Memory *memoryPointer;
		uint16 opcode;

		int executeOpcode();
		uint16 fetchOpcode();


	//public:
		CPU(Memory *memP);
		void emulateCycle();
		static int opcodeCycles[256];
		static int opcodeSize[256];
		static int opcodeMode[256];

}

#endif
