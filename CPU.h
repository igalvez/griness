#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "types.h"
#include "Memory.h"
#include <stdio.h>

//using namespace std;

#ifndef CPU_H
#define CPU_H

enum Pflags{
	C,    // Carry
	Z,    // Zero
	I,    // Interrupt inhibit, 0: IRQ and NMI get through, 1: just NMI
	D,    // Decimal
	B,    // Break flag? not used
	U,    // Always set (unsigned?)
	V,    // Overflow
	N     // Negative
};

class CPU{
	public:
		uint16 pc; // program counter
		uint16 sp;  // stack pointer
		uint8 A;   // accumulator
		uint8 X;  // index register X
		uint8 Y;  // index register Y
		uint8 P;  // Processor status - <NV-BDIZC>
		uint8 status[8];
		char stat_chart[8];// = {'n','v','u','b','d','i','z','c'};
		uint8 test_stat;


		//flags
		/*
		uint8 N; Negative flag
		uint8 Z; Zero flag
		uint8 C; Carry flagO
		uint8 I; Interrupt disable flag
		uint8 B; Break flag
		uint8 D; // not used
		uint8 V; Overflow flag
		*/
		Memory *memory;
		uint16 opcode;
		unsigned int addr_brk; // For debugging: create a breakpoint on specific address
		uint16 operand; // Help variable to store operands used by instruction
		uint16 reset_vector;
		bool on_nmi;
		int executeOpcode();
		int fetchOpcode(std::string *str);

		int cclock;
		std::string debug_str;

	//public:
		CPU();
		void initialize(Memory *memP);
		int emulateCycles(int cycles, std::string *str);

		uint8 comp2Operation(uint8 n1, uint8 n2, char op='+', bool withcarry=false);
		uint8 sum_operation(uint8 n1, uint8 n2, uint8 carry=0);
		uint8 sub_operation(uint8 n1, uint8 n2, uint8 carry=0);
		uint8 check_overflow (uint8 n1, uint8 n2, uint8 res);
		inline void updateSP(int increase);

		void executeNMI();
		static int opcodeCycles[256];
		static int opcodePageCycles[256];
		static int opcodeSize[256];
		static int opcodeMode[256];
		static std::string opcodeNameTable[256];

		void compareElements(uint8 reg);
		void store_value(int addr, uint8 value);
		uint8 get_value(int addr);
		uint8 getPflag(int flagPos);
		void setPflag(int flagPos, uint8 value);
		void reset();
		int branch();


		void push_status_to_Stack(bool isInt);
		void pop_status_from_Stack();
		void setSignalFlags(uint8 value);

};

#endif
