#include "CPU.h"



/*  M = memory
   	A = A register ( Accumulator)
	X = X register
	Y = Y register
*/
   
/****** Storage functions ******/

int ldaF(CPU *cpuObj){
// Load A with M

}

int ldxF(CPU *cpuObj){
// Load X with M
}

int ldyF(CPU *cpuObj){
// Load Y with M
}

int staF(CPU *cpuObj){
// Store A in M
}

int stxF(CPU *cpuObj){
// Store X in M
}

int styF(CPU *cpuObj){
// Store Y in M
}

int taxF(CPU *cpuObj){
// Transfer A to X
	cpuObj->X = cpuObj->A;
	cpuObjs->pc++;
}

int tayF(CPU *cpuObj){
// Transfer A to Y
	cpuObj->Y = cpuObj->A;
	cpuObjs->pc++;
}

int tsxF(CPU *cpuObj){
// Transfer Stack Pointer to X
	cpuObj->X = cpuObj->sp;
	cpuObjs->pc++;
}

int txaF(CPU *cpuObj){
// Transfer X to A
	cpuObj->A = cpuObj->X;
	cpuObjs->pc++;
}

int txsF(CPU *cpuObj){
//Transfer X to Stack Pointer
	cpuObj->sp = cpuObj->X;
	cpuObjs->pc++;
}

int tyaF(CPU *cpuObj){
// Transfer Y to A
	cpuObj->A = cpuObj->Y;
	
}

/****** Math Functions ******/

int adcF(CPU *cpuObj){
// Add Memory to Accumulator with Carry
}

int decF(CPU *cpuObj){
// Decrement M by One
}

int dexF(CPU *cpuObj){
// Decrement X by One
}

int deyF(CPU *cpuObj){
//  Decrement Y by One
}

int incF(CPU *cpuObj){
// Increment M by One
}

int inxF(CPU *cpuObj){
// Increment X by One
}

int inyF(CPU *cpuObj){
// Increment Y by One
}

int sbcF(CPU *cpuObj){
// Subtract M from A with Borrow
}

/****** Bitwise operation functions ******/

int andF(CPU *cpuObj){
//  "AND" M with A
}

int aslF(CPU *cpuObj){
// Shift Left One Bit (M or A)
}

int bitF(CPU *cpuObj){
// Test Bits in M with A
}

int eorF(CPU *cpuObj){
// "Exclusive-Or" M with A
}

int lsrF(CPU *cpuObj){
// Shift Right One Bit (M or A)
}

int oraF(CPU *cpuObj){
// "OR" M with A
}

int rolF(CPU *cpuObj){
// Rotate One Bit Left (M or A)
}

int rorF(CPU *cpuObj){
// Rotate One Bit Right (M or A)
}

/****** Branch functions ******/

int bccF(CPU *cpuObj){
// Branch on Carry Clear
}

int bcsF(CPU *cpuObj){
// Branch on Carry Set
}

int beqF(CPU *cpuObj){
// Branch on Result Zero
}

int bmiF(CPU *cpuObj){
// Branch on Result Minus
}

int bneF(CPU *cpuObj){
// Branch on Result not Zero
}

int bplF(CPU *cpuObj){
// Branch on Result Plus
}

int bvcF(CPU *cpuObj){
// Branch on Overflow Clear
}

int bvsF(CPU *cpuObj){
// Branch on Overflow Set
}

/****** Jump functions ******/

int jmpF(CPU *cpuObj){
//  Jump to Location
}

int jsrF(CPU *cpuObj){
// Jump to Location Save Return Address
}

int rtiF(CPU *cpuObj){
// Return from Interrupt
}

int rtsF(CPU *cpuObj){
// Return from Subroutine
}

/****** Register operation functions ******/

int clcF(CPU *cpuObj){
// Clear Carry Flag
}

int cldF(CPU *cpuObj){
// Clear Decimal Mode
}

int cliF(CPU *cpuObj){
// Clear interrupt Disable Bit
}

int clvF(CPU *cpuObj){
// Clear Overflow Flag
}

int cmpF(CPU *cpuObj){
// Compare M and A
}

int cpxF(CPU *cpuObj){
// Compare M and X
}

int cpyF(CPU *cpuObj){
// Compare M and Y
}

int secF(CPU *cpuObj){
// Set Carry Flag
}

int sedF(CPU *cpuObj){
// Set Decimal Mode
}

int seiF(CPU *cpuObj){
// Set Interrupt Disable Status
}

/****** Stack functions ******/

int phaF(CPU *cpuObj){
//Push A on Stack
}

int phpF(CPU *cpuObj){
// Push Processor Status on Stack
}

int plaF(CPU *cpuObj){
// Pull A from Stack
}

int plpF(CPU *cpuObj){
// Pull Processor Status from Stack
}

/****** System functions ******/

int brkF(CPU *cpuObj){
// Force Break
}

int nopF(CPU *cpuObj){
// No Operation
	return 0;
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
	opcode = memory[pc];
	
	switch
}
