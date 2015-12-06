#include "CPU.h"



enum Modes{
	accumulator,
	immediate,
	zpage,
	zpageX,
	zpageY,
	absolute,
	absoluteX,
	absoluteY,
	implied,
	relative,
	indirectX,
	indirectY,
	absoluteIndirect
};

	
/*  M = memory
   	A = A register ( Accumulator)
	X = X register
	Y = Y register
*/
   
/* ALFABETICAL ORDER */


int adcF(CPU *cpuObj){
// Add Memory to Accumulator with Carry
	if(CPU.opcodeMode[cpuObj->opcode] == immediate){
		if((cpuObj->A + cpuObj->operand + cpuObj->carry)>0xFF){
			cpuObj->A += cpuObj->operand + cpuObj->carry;
		}
	else{
		if((cpuObj->A + cpuObj->memory[operand] + cpuObj->carry)>0xFF){
			cpuObj->A += cpuObj->memory[operand] + cpuObj->carry;
		}

	}
}

int andF(CPU *cpuObj){
//  "AND" M with A
	if(CPU.opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->A &= cpuObj->operand;			
	}
	else{
		cpuObj->A &= cpuObj->memory[cpuObj->operand];
	}
}

int aslF(CPU *cpuObj){
// Shift Left One Bit (M or A)
	if(CPU.opcodeMode[cpuObj->opcode] == accumulator){
		cpuObj->carry = cpuObj->A >> 7;
		cpuObj->A = cpuObj->A << 1;
	}
	else{
		cpuObj->carry = cpuObj->memory[cpuObj->operand] >> 7;
		cpuObj->memory[cpuObj->operand] = cpuObj->memory[cpuObj->operand] << 1;
	}
}

int bccF(CPU *cpuObj){
// Branch on Carry Clear
	int operand = cpuObj->operand;
	if(cpuObj->carry==0){
		cpuObj->pc += operand;
	}
}

int bcsF(CPU *cpuObj){
// Branch on Carry Set
	int operand = cpuObj->operand;
	if(cpuObj->carry==1){
		cpuObj->pc += operand;
	}
}

int beqF(CPU *cpuObj){
// Branch on Result Zero
	int operand = cpuObj->operand;
	if(cpuObj->Z==1){
		cpuObj->pc += operand;
	}
}

int bitF(CPU *cpuObj){
// Test Bits in M with A
	int operand = cpuObj->operand;
	if((cpuObj->A & cpuObj->memory[operand]) == 0){
		cpuObj->Z = 1;
	}
	else{
		cpuObj->N = cpuObj->memory[operand] >> 7;
		cpuOnj->V = (cpuObj->memory[operand] & 0x40) >> 6;
	}
}

int bmiF(CPU *cpuObj){
// Branch on Result Minus
	if(cpuObj->N==1){
		cpuObj->pc += cpuObj->operand;
	}
}

int bneF(CPU *cpuObj){
// Branch on Result not Zero
	if(cpuObj->Z==0){
		cpuObj->pc += cpuObj->operand;
	}
}

int bplF(CPU *cpuObj){
// Branch on Result Plus
	if(cpuObj->N==0){
		cpuObj->pc += cpuObj->operand;
	}
}

int brkF(CPU *cpuObj){
// Force Break
	TODO
}

int bvcF(CPU *cpuObj){
// Branch on Overflow Clear
	if(cpuObj->V==0){
		cpuObj->pc += cpuObj->operand;
	}
}

int bvsF(CPU *cpuObj){
// Branch on Overflow Set
	if(cpuObj->V==1){
		cpuObj->pc += cpuObj->operand;
	}
}

int clcF(CPU *cpuObj){
// Clear Carry Flag
	cpuObj->C = 0;
}

int cldF(CPU *cpuObj){
// Clear Decimal Mode
	cpuObj->D = 0;
}

int cliF(CPU *cpuObj){
// Clear interrupt Disable Bit
	cpuObj->I = 0;
}

int clvF(CPU *cpuObj){
// Clear Overflow Flag
	cpuObj->V = 0;
}

int cmpF(CPU *cpuObj){
// Compare M and A : A - M 
	cpuObj->compareElements(cpuObj->A);
}

int cpxF(CPU *cpuObj){
// Compare M and X
	cpuObj->compareElements(cpuObj->X);
}

int cpyF(CPU *cpuObj){
// Compare M and Y
	cpuObj->compareElements(cpuObj->Y);
}

int decF(CPU *cpuObj){
// Decrement M by One
	cpuObj->memory[operand] -= 1;
}

int dexF(CPU *cpuObj){
// Decrement X by One
	cpuObj->X -= 1;
}

int deyF(CPU *cpuObj){
//  Decrement Y by One
	cpuObj->Y -= 1;
}

int eorF(CPU *cpuObj){
// "Exclusive-Or" M with A
	int number;
	if(CPU.opcodeMode[cpuObj->opcode] == immediate){
		number = cpuObj->operand;
	}
	else{
		number = cpuObj->memory[cpuObj->operand];
	}

	cpuObj->A ^= number;
}

int incF(CPU *cpuObj){
// Increment M by One
	cpuObj->memory[operand] += 1;
}

int inxF(CPU *cpuObj){
// Increment X by One
	cpuObj->X += 1;
}

int inyF(CPU *cpuObj){
// Increment Y by One
	cpuObj->Y += 1;
}

int jmpF(CPU *cpuObj){
//  Jump to Location
	cpuObj->pc = cpuObj.memory[operand];
}

int jsrF(CPU *cpuObj){
// Jump to Location Save Return Address
	TODO
}

int ldaF(CPU *cpuObj){
// Load A with M
	if(CPU.opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->A = cpuObj->operand;
	}
	else{
		cpuObj->A = cpuObj->memory[cpuObj->operand];
	}

}

int ldxF(CPU *cpuObj){
// Load X with M
	if(CPU.opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->X = cpuObj->operand;
	else{
		cpuObj->X = cpuObj->memory[cpuObj->operand];
	}
}

int ldyF(CPU *cpuObj){
// Load Y with M
	if(CPU.opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->Y = cpuObj->operand;		
	}
	else{
		cpuObj->Y = cpuObj->memory[cpuObj->operand];
	}
}

int lsrF(CPU *cpuObj){
// Shift Right One Bit (M or A)
	if(CPU.opcodeMode[cpuObj->opcode] == accumulator){
		cpuObj->C = (cpuObj->A & 0x01);
		cpuObj->A = cpuObj->A >> 1;
	}
	else{
		cpuObj->C = (cpuObj->memory[cpuObj->operand] & 0x01);
		cpuObj->memory[cpuObj->operand] = cpuObj->memory[cpuObj->operand] >> 1;
	}
}

int nopF(CPU *cpuObj){
// No Operation
	return 0;
}

int oraF(CPU *cpuObj){
// "OR" M with A
	if(CPU.opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->A |= cpuObj->operand;
	}
	else{
		cpuObj->A |= cpuObj->memory[cpuObj->operand];
	}
}


int phaF(CPU *cpuObj){
//Push A on Stack
	cpuObj->memory[cpuObj->sp--] = cpuObj->A;
	//--cpuObj->sp;
}

int phpF(CPU *cpuObj){
// Push Processor Status on Stack
	cpuObj->memory[cpuObj->sp--] = cpuObj->P;
}

int plaF(CPU *cpuObj){
// Pull A from Stack
	cpuObj->A = cpuObj->memory[cpuObj->sp++];
}

int plpF(CPU *cpuObj){
// Pull Processor Status from Stack
	cpuObj->P = cpuObjs->memory[cpuObj->sp++];
}

int rolF(CPU *cpuObj){
// Rotate One Bit Left (M or A)
	TODO
	if(CPU.opcodeMode[cpuObj->opcode] == accumulator){
				
	}
	else{

	}
}

int rorF(CPU *cpuObj){
// Rotate One Bit Right (M or A)
	int aux;
	if(CPU.opcodeMode[cpuObj->opcode] == accumulator){
		aux = (cpuObj->A & 0x01);
		cpuObj->A = cpuObj->A >> 1;
		cpuObj->A |= (cpuObj->C << 7);
	}
	else{
		aux = (cpuObj->memory[operand] & 0x01);
		cpuObj->memory[operand] = cpuObj->memory[operand] >> 1;
		cpuObj->memory[operand] |= (cpuObj->C << 7);
	}
	cpuObj->C = aux;
}
int rtiF(CPU *cpuObj){
// Return from Interrupt
	TODO
}

int rtsF(CPU *cpuObj){
// Return from Subroutine
	cpuObj->pc = cpuObj->memory[cpuObj->sp++];
}

int sbcF(CPU *cpuObj){
// Subtract M from A with Borrow
	int number;
	if(CPU.opcodeMode[cpuObj->opcode] == immediate){
		number = cpuObj->operand;
	}
	else{
		number = cpuObj->memory[operand];
	}
	cpuObj->A -= (number + cpuObj->C);
}

int secF(CPU *cpuObj){
// Set Carry Flag
	cpuObj->C = 1;
}

int sedF(CPU *cpuObj){
// Set Decimal Mode
	cpuObj->D = 1;
}

int seiF(CPU *cpuObj){
// Set Interrupt Disable Status
	cpuObj->I = 1;
}

int staF(CPU *cpuObj){
// Store A in M
	cpuObj->memory[operand] = cpuObj->A;
}

int stxF(CPU *cpuObj){
// Store X in M
	cpuObj->memory[operand] = cpuObj->X;
}

int styF(CPU *cpuObj){
// Store Y in M
	cpuObj->memory[operand] = cpuObj->Y;
}

int taxF(CPU *cpuObj){
// Transfer A to X
	cpuObj->X = cpuObj->A;
}

int tayF(CPU *cpuObj){
// Transfer A to Y
	cpuObj->Y = cpuObj->A;
}

int tsxF(CPU *cpuObj){
// Transfer Stack Pointer to X
	cpuObj->X = cpuObj->sp;
}

int txaF(CPU *cpuObj){
// Transfer X to A
	cpuObj->A = cpuObj->X;
}

int txsF(CPU *cpuObj){
//Transfer X to Stack Pointer
	cpuObj->sp = cpuObj->X;
}

int tyaF(CPU *cpuObj){
// Transfer Y to A
	cpuObj->A = cpuObj->Y;
	
}





void CPU::compareElements(uint8 reg){
	uint8 number;
	if(opcodeMode[opcode] == immediate){
		number = operand
	}
	else{
		number = memory[operand];
	}
	C = 0;
	if((reg - number)<0){
		N = 1;
		Z = 0;
	}
	else if((reg - number) == 0){
		N = 0;
		Z = 1;	
	}
	else{
		N = 0;
		Z = 0;
	}
}




int (*jumpTable[256])(CPU *cpuObj) ={
	brkF, oraF, NULL, NULL, NULL, oraF, aslF, NULL, phpF, oraF, aslF, NULL, NULL, oraF, aslF, NULL, //0
	bplF, oraF, NULL, NULL, NULL, oraF, aslF, NULL, clcF, oraF, NULL, NULL, NULL, oraF, aslF, NULL, //1
	jsrF, andF, NULL, NULL, bitF, andF, rolF, NULL, plpF, andF, rolF, NULL, bitF, andF, rolF, NULL, //2
	bmiF, andF, NULL, NULL, NULL, andF, rolF, NULL, secF, andF, NULL, NULL, NULL, andF, rolF, NULL, //3
	rtiF, eorF, NULL, NULL, NULL, eorF, lsrF, NULL, phaF, eorF, lsrF, NULL, jmpF, eorF, lsrF, NULL, //4
	bvcF, eorF, NULL, NULL, NULL, eorF, lsrF, NULL, cliF, eorF, NULL, NULL, NULL, eorF, lsrF, NULL, //5
	rtsF, adcF, NULL, NULL, NULL, adcF, rorF, NULL, plaF, adcF, rorF, NULL, jmpF, adcF, rorF, NULL, //6
	bvsF, adcF, NULL, NULL, NULL, adcF, rorF, NULL, seiF, adcF, NULL, NULL, NULL, adcF, rorF, NULL, //7
	NULL, staF, NULL, NULL, styF, staF, stxF, NULL, deyF, NULL, txaF, NULL, styF, staF, stxF, NULL, //8
	bccF, staF, NULL, NULL, styF, staF, stxF, NULL, tyaF, staF, txsF, NULL, NULL, staF, NULL, NULL, //9
	ldyF, ldaF, ldxF, NULL, ldyF, ldaF, ldxF, NULL, tayF, ldaF, taxF, NULL, ldyF, ldaF, ldxF, NULL, //A
	bcsF, ldaF, NULL, NULL, ldyF, ldaF, ldxF, NULL, clvF, ldaF, tsxF, NULL, ldyF, ldaF, ldxF, NULL, //B
	cpyF, cmpF, NULL, NULL, cpyF, cmpF, decF, NULL, inyF, cmpF, dexF, NULL, cpyF, cmpF, decF, NULL, //C
	bneF, cmpF, NULL, NULL, NULL, cmpF, decF, NULL, cldF, cmpF, NULL, NULL, NULL, cmpF, decF, NULL, //D
	cpxF, sbcF, NULL, NULL, cpxF, sbcF, incF, NULL, inxF, sbcF, nopF, NULL, cpxF, sbcF, incF, NULL, //E
	beqF, sbcF, NULL, NULL, NULL, sbcF, incF, NULL, sedF, sbcF, NULL, NULL, NULL, sbcF, incF, NULL  //F
	//0 ,  1  ,  2  ,  3  ,  4  ,  5  ,  6  ,  7  ,  8  ,  9  ,  A  ,  B  ,  C  ,  D  ,  E  ,  F  

};

static int CPU::opcodeCycles[256] ={
	7, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 0, 4, 6, 0, //0
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, //1
	6, 6, 0, 0, 3, 3, 5, 0, 4, 2, 2, 0, 4, 4, 6, 0, //2
};
static int CPU::opcodeSize[256] = {

};
static int CPU::opcodeMode[256] = {

};

int CPU::executeOpcode(){
	jumpTable[opcode](this);
}

CPU::CPU(Memory *memP){
	memoryPointer = memP;
}

void CPU::emulateCycle(){

}

uint16 CPU::fetchOpcode(){

	int mode;
	opcode = memory[pc];
	mode = opcodeMode[opcode]

	/*

	accumulator,
	immediate,
	zpage,
	zpageX,
	zpageY,
	absolute,
	absoluteX,
	absoluteY,
	implied,
	relative,
	indirectX,
	indirectY,
	absoluteIndirect
	*/
	switch(mode):
		case accumulator:
			// Accumulator
			operand = -2;
			jumpTable[opcode](self);
			pc+=2;
			break;
		case immediate:
			// Immediate
			operand = memory[pc+1];
			jumpTable[opcode](self);
			pc+=2;
			break;
		case zpage:
			// Zero Page
			operand = memory[pc+1];
			jumpTable[opcode](self);
			pc+=2;
			break;
		case zpageX:
			// Indexed Zero Page with X
			operand = memory[pc+1] + X;
			jumpTable[opcode](self);
			pc+=2;
			break;
		case zpageY:
			// Indexed Zero Page with Y
			operand = memory[pc+1] + Y;
			jumpTable[opcode](self);
			pc+=2;
			break;
		case absolute:
			// Absolute
		{
			int less_sig = memory[pc+1];
			int more_sig = memory[pc+2] << 8;
			operand = more_sig | less_sig;
			jumpTable[opcode](self);
			pc+=3;
		}
			break;
		case absoluteX:
			// Indexed Absolute X:
		{
			int less_sig = memory[pc+1];
			int more_sig = memory[pc+2];
			operand = more_sig | less_sig;
			operand += X;
			jumpTable[opcode](self);
			pc+=3;
		}
			break;
		case absoluteY:
			// Indexed Absolute Y	
		{
			int less_sig = memory[pc+1];
			int more_sig = memory[pc+2];
			operand = more_sig | less_sig;
			operand += X;
			jumpTable[opcode](self);
			pc+=3;
		}
			break;	
		case implied:
			// Implied
			operand = -1;
			jumpTable[opcode](self);
			pc+=1;
		case relative:
			operand = memory[pc+1];
			jumpTable[opcode](self);
			pc+=2;
		case indirectX:
			// Pre-indexed indirect, X
		{
			int less_sig = memory[memory[pc+1]+X];
			int more_sig = memory[memory[pc+1]+X+1] << 8;
			operand = more_sig | less_sig;
			jumpTable[opcode](self);
			pc+=2;
		}
		case indirectY:
			// Post-indexed indirect, Y
		{
			int less_sig = memory[memory[pc+1]];
			int more_sig = memory[memory[pc+1]+1] << 8;
			operand = more_sig | less_sig;
			operand += Y;
			jumpTable[opcode](self);
			pc+=2;
		}	
		case absoluteIndirect:
			// Absolute Indirect	
		{
			int pointer = (memory[pc+2] << 8) | memory[pc+1];
			operand = (memory[pointer+1] << 8) | memory[pointer];
			jumpTable[opcode](self);
			pc+=3;
		}
	}
}
