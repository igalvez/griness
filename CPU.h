
#include "utils.h"


#ifndef CPU_H
#define CPU_H

Class CPU{
	private:
		uint16 pc; // program counter
		uint8 sp;  // stack pointer
		uint8 A;   // accumulator
		uint8 X;  // index register X
		uint8 Y;  // index register Y
		uint8 P;  // Processor status
		Memory *memoryPointer;

		static int (*jumpTable[256])();
		static int opcodeCycles[256];
		static int opcodeSize[256];
		static int opcodeMode[256];

	public:
		CPU(Memory *memP);
		void emulateCycle();



}

#endif