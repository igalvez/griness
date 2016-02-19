#include "types.h"
#include "Memory.h"
#include <stdio.h>

using namespace std;

#ifndef CPU_H
#define CPU_H

enum Pflags{
	C,
	Z,
	I,
	D,
	B,
	V=6,
	N
};
	

class CPU{
	public:
		uint16 pc; // program counter
		uint16 sp;  // stack pointer
		uint8 A;   // accumulator
		uint8 X;  // index register X
		uint8 Y;  // index register Y
		uint8 P;  // Processor status - <NV-BDIZC>

		//flags
		/*
		uint8 N; Negative flag
		uint8 Z; Zero flag
		uint8 C; Carry flag
		uint8 I; Interrupt disable flag
		uint8 B; Break flag
		uint8 D; // not used
		uint8 V; Overflow flag
		*/
		Memory *memory;
		uint16 opcode;
		uint16 operand; // Help variable to store operands used by instruction
		uint16 reset_vector;
		int executeOpcode();
		int fetchOpcode();


	//public:
		CPU();
		void initialize(Memory *memP);
		int emulateCycle();
		static int opcodeCycles[256];
		static int opcodeSize[256];
		static int opcodeMode[256];
		
		void compareElements(uint8 reg);
		void store_value(int addr, uint8 value);
		uint8 get_value(int addr);
		uint8 getPflag(int flagPos);
		void setPflag(int flagPos, uint8 value);
		void reset();
		void branch();
		

};

#endif
