#include "CPU.h"
#define PRG_OFFSET 0x8000

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


void adcF(CPU *cpuObj){ // 
// Add Memory to Accumulator with Carry
	uint8 carry = cpuObj->getPflag(C);
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){ //#69
		if((cpuObj->A + cpuObj->operand + carry)>0xFF){
			cpuObj->A += cpuObj->operand + carry;
		}
	}
	else{  
	/* ZPAGE:#65, ZPAGEX:#75, ABSOLUTE:#6D, ABSOLUTEX:#7D
	   ABSOLUTEY:#79, INDIRECTX:#61, INDIRECTY:#71
	*/
		if((cpuObj->A + cpuObj->memory->read(cpuObj->operand) + carry)>0xFF){
			cpuObj->A += cpuObj->memory->read(cpuObj->operand) + carry;
		}
	}
}

void andF(CPU *cpuObj){
//  "AND" M with A
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){ //#29
		cpuObj->A &= cpuObj->operand;			
	}
	else{
		/* ZPAGE:#25, ZPAGEX:#35, ABSOLUTE:#2D, ABSOLUTEX:#3D
		   ABSOLUTEY:#39, INDIRECTX:#21, INDIRECTY:#31 */
		cpuObj->A 	&= cpuObj->memory->read(cpuObj->operand);
	}
}

void aslF(CPU *cpuObj){
// Shift Left One Bit (M or A)
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){ //#0A
		cpuObj->setPflag(C,cpuObj->A >> 7);
		cpuObj->A = cpuObj->A << 1;
	}
	else{
		/* ZPAGE:#06, ZPAGEX:#16, ABSOLUTE:#0E, ABSOULTEX:#1E */
		cpuObj->setPflag(C, cpuObj->memory->read(cpuObj->operand) >> 7);
		cpuObj->memory->write(cpuObj->operand, cpuObj->memory->read(cpuObj->operand) << 1);
	}
}

void bccF(CPU *cpuObj){ //#90
// Branch on C Clear
	int operand = cpuObj->operand;
	if(cpuObj->getPflag(C)==0){
		cpuObj->branch();;
	}
}

void bcsF(CPU *cpuObj){ //#B0
// Branch on C Set
	int operand = cpuObj->operand;
	if(cpuObj->getPflag(C)==1){
		cpuObj->branch();;
	}
}

void beqF(CPU *cpuObj){//#F0
// Branch on Result Zero
	int operand = cpuObj->operand;
	if(cpuObj->getPflag(Z)==1){
		cpuObj->branch();;
	}
}

void bitF(CPU *cpuObj){
// Test Bits in M with A
	/* ZPAGE:#24, ABSOLUTE:#2C */
	uint8 memV = cpuObj->memory->read(cpuObj->operand);
	int operand = cpuObj->operand;
	if((cpuObj->A & memV) == 0){
		cpuObj->setPflag(Z,1);
	}
	else{
		cpuObj->setPflag(N,memV >> 7);
		cpuObj->setPflag(V,(memV & 0x40) >> 6);
	}
}

void bmiF(CPU *cpuObj){ //#30
// Branch on Result Minus
	if(cpuObj->getPflag(N)==1){
		cpuObj->branch();
	}
}

void bneF(CPU *cpuObj){ //#D0
// Branch on Result not Zero
	if(cpuObj->getPflag(Z)==0){
		cpuObj->branch();
	}
}

void bplF(CPU *cpuObj){ //#10
// Branch on Result Plus
	printf("BPL operand %d \n", cpuObj->operand); 
	if(cpuObj->getPflag(N)==0){
		cpuObj->branch();
	}
}



void brkF(CPU *cpuObj){ //#00
// Force Break
	//cpuObj->memory->write(cpuObj->sp--, cpuObj->pc);

	uint8 pc_high = (cpuObj->pc+2 & 0xFF00) >> 8;
	uint8 pc_low = cpuObj->pc+2 & 0x00FF;
	cpuObj->memory->write(cpuObj->sp--, pc_high);
	cpuObj->memory->write(cpuObj->sp--, pc_low);
	cpuObj->memory->write(cpuObj->sp--, cpuObj->P);
	cpuObj->pc = ((cpuObj->memory->read(0xfffe)<<8) | cpuObj->memory->read(0xffff)) -1;
	cpuObj->setPflag(B,1);
	//cpuObj->pc = cpuObj->memory->read(cpuObj->operand) + PRG_ADDR - 3;
}

void bvcF(CPU *cpuObj){ //#50
// Branch on Overflow Clear
	if(cpuObj->getPflag(V)==0){
		cpuObj->branch();
	}
}

void bvsF(CPU *cpuObj){ //#70
// Branch on Overflow Set
	if(cpuObj->getPflag(V)==1){
		cpuObj->branch();
	}
}

void clcF(CPU *cpuObj){ //#18
// Clear C Flag
	cpuObj->setPflag(C,0);
}

void cldF(CPU *cpuObj){ //#D8
// Clear Decimal Mode
	cpuObj->setPflag(D,0);
}

void cliF(CPU *cpuObj){ //#58
// Clear interrupt Disable Bit
	cpuObj->setPflag(I,0);
}

void clvF(CPU *cpuObj){ //#B8
// Clear Overflow Flag
	cpuObj->setPflag(V,0);
}

void cmpF(CPU *cpuObj){
// Compare M and A : A - M
/* IMMEDIATE:#C9, ZPAGE:#C5, ZPAGEX:#D5, ABSOLUTE:#CD, ABSOLUTEX:#DD
   ABSOLUTEY:#D9, INDIRECTX:#C1, INDIRECTY:#D1 */ 
	cpuObj->compareElements(cpuObj->A);
}

void cpxF(CPU *cpuObj){
// Compare M and X
/* IMMEDIATE:#E0, ZPAGE:#E4, ABSOLUTE:#EC */ 
	cpuObj->compareElements(cpuObj->X);
}

void cpyF(CPU *cpuObj){
// Compare M and Y
/* IMMEDIATE:#C0, ZPAGE:#C4, ABSOLUTE:#CC */ 
	cpuObj->compareElements(cpuObj->Y);
}

void decF(CPU *cpuObj){
// Decrement M by One
/* ZPAGE:#C6, ZPAGEX:#D6, ABSOLUTE:#CE, ABSOLUTEX:#DE */ 
	uint8 memV = cpuObj->memory->read(cpuObj->operand);
	cpuObj->memory->write(cpuObj->operand, memV - 1);
}

void dexF(CPU *cpuObj){ //#CA
// Decrement X by One
	cpuObj->X -= 1;
}

void deyF(CPU *cpuObj){ //#88
//  Decrement Y by One
	cpuObj->Y -= 1;
}

void eorF(CPU *cpuObj){
// "Exclusive-Or" M with A
/* IMMEDIATE:#49, ZPAGE:#45, ZPAGEX:#55, ABSOLUTE:#4D, ABSOLUTEX:#5D
   ABSOLUTEY:#59, INDIRECTX:#41, INDIRECTY:#51 */ 
	int number;
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		number = cpuObj->operand;
	}
	else{
		number = cpuObj->memory->read(cpuObj->operand);
	}

	cpuObj->A ^= number;
}

void incF(CPU *cpuObj){ 
// Increment M by One
/* ZPAGE:#E6, ZPAGEX:#F6, ABSOLUTE:#EE, ABSOLUTEX:#FE */ 
	int memV = cpuObj->memory->read(cpuObj->operand);
	cpuObj->memory->write(cpuObj->operand, memV + 1);
}

void inxF(CPU *cpuObj){ //#E8
// Increment X by One
	cpuObj->X += 1;
}

void inyF(CPU *cpuObj){ //#C8
// Increment Y by One
	cpuObj->Y += 1;
}

void jmpF(CPU *cpuObj){ 
//  Jump to Location
/* INDIRECT:#6C, ABSOLUTE:#4C */

	cpuObj->pc =  cpuObj->operand - 3;

}

void jsrF(CPU *cpuObj){ //#20
// Jump to Location Save Return Address
	uint8 pc_high = (cpuObj->pc+2 & 0xFF00) >> 8;
	uint8 pc_low = (cpuObj->pc+2) & 0x00FF;
	cpuObj->memory->write(cpuObj->sp--, pc_high);
	cpuObj->memory->write(cpuObj->sp--, pc_low);
	cpuObj->pc = cpuObj->operand - 3;
	for (int i=cpuObj->sp; i<=0x1ff; i++){
		printf("%x = %x\n", i, cpuObj->memory->read(i));
	}
}

void ldaF(CPU *cpuObj){
// Load A with M
/* IMMEDIATE:#A9, ZPAGE:#A5, ZPAGEX:#B5, ABSOLUTE:#AD, ABSOLUTEX:#BD
   ABSOLUTEY:#B9, INDIRECTX:#A1, INDIRECTY:#B1 */ 
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		printf("cpuOBJ->A before = %x\n", cpuObj->A);
		cpuObj->A = (short) cpuObj->operand;
		printf("operandeeee = %x\n", cpuObj->operand);
		printf("cpuOBJ->A = %x\n", cpuObj->A);
	}
	else{
		printf("MEM TO A = %x",cpuObj->memory->read(cpuObj->operand)); 

		cpuObj->A = cpuObj->memory->read(cpuObj->operand);
	}

}

void ldxF(CPU *cpuObj){
// Load X with M
/* IMMEDIATE:#A2, ZPAGE:#A6, ZPAGEY:#B6, ABSOLUTE:#AE, ABSOLUTEY:#BE */ 
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->X = cpuObj->operand;
	}
	else{
		cpuObj->X = cpuObj->memory->read(cpuObj->operand);
	}
}

void ldyF(CPU *cpuObj){
// Load Y with M
/* IMMEDIATE:#A0, ZPAGE:#A4, ZPAGEX:#B4, ABSOLUTE:#AC, ABSOLUTEX:#BC */ 
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->Y = cpuObj->operand;		
	}
	else{
		cpuObj->Y = cpuObj->memory->read(cpuObj->operand);
	}
}

void lsrF(CPU *cpuObj){
// Shift Right One Bit (M or A)
/* ACCUMULATOR:#4A, ZPAGE:#46, ZPAGEX:#56, ABSOLUTE:#4E, ABSOLUTEX:#5E */ 
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		cpuObj->setPflag(C,(cpuObj->A & 0x01));
		cpuObj->A = cpuObj->A >> 1;
	}
	else{
		int memV = cpuObj->memory->read(cpuObj->operand);
		cpuObj->setPflag(C,(memV & 0x01));
		cpuObj->memory->write(cpuObj->operand, memV >> 1);
	}
}

void nopF(CPU *cpuObj){ //#EA
// No Operation
	return;
}

void oraF(CPU *cpuObj){
// "OR" M with A
/* IMMEDIATE:#09, ZPAGE:#05, ZPAGEX:#15, ABSOLUTE:#0D, ABSOLUTEX:#1D
   ABSOLUTEY:#19, INDIRECTX:#01, INDIRECTY:#11 */ 
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->A |= cpuObj->operand;
	}
	else{
		cpuObj->A |= cpuObj->memory->read(cpuObj->operand);
	}
}


void phaF(CPU *cpuObj){ //#48
//Push A on Stack
	cpuObj->memory->write(cpuObj->sp--, cpuObj->A);
	//--cpuObj->sp;
}

void phpF(CPU *cpuObj){ //#08
// Push Processor Status on Stack
	cpuObj->memory->write(cpuObj->sp--, cpuObj->P);
}

void plaF(CPU *cpuObj){ //#68
// Pull A from Stack
	cpuObj->A = cpuObj->memory->read(++cpuObj->sp);
}

void plpF(CPU *cpuObj){ //#28
// Pull Processor Status from Stack
	cpuObj->P = cpuObj->memory->read(++cpuObj->sp);
}

void rolF(CPU *cpuObj){ 
// Rotate One Bit Left (M or A)
/* ACCUMULATOR:#2A, ZPAGE:#26, ZPAGEX:#36, ABSOLUTE:#2E, ABSOLUTEX:#3E */ 
	int aux;
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		aux = (cpuObj->A & 0x80);
		cpuObj->A = cpuObj->A << 1;
		cpuObj->A |= cpuObj->getPflag(C);				
	}
	else{
		uint8 memV = cpuObj->memory->read(cpuObj->operand);	
		int n = memV << 1 | cpuObj->getPflag(C);
		aux = (memV & 0x80);
		cpuObj->memory->write(cpuObj->operand, n);
		
		//*cpuObj->memory->write(cpuObj->operand] = memV << 1;
		//*cpuObj->memory->map[cpuObj->operand] |= cpuObj->C;		
	}
	cpuObj->setPflag(C,aux);
}

void rorF(CPU *cpuObj){
// Rotate One Bit Right (M or A)
/* ACCUMULATOR:#6A, ZPAGE:#66, ZPAGEX:#76, ABSOLUTE:#6E, ABSOLUTEX:#7E */ 
	int aux;
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		aux = (cpuObj->A & 0x01);
		cpuObj->A = cpuObj->A >> 1;
		cpuObj->A |= (cpuObj->getPflag(C) << 7);
	}
	else{
		int memV = cpuObj->memory->read(cpuObj->operand);
		int n = (cpuObj->getPflag(C) << 7) | (memV >> 1);
		aux = (memV & 0x01);
		cpuObj->memory->write(cpuObj->operand, n);
		//*cpuObj->memory->map[cpuObj->operand] |= (cpuObj->C << 7);
	}
	cpuObj->setPflag(C,aux);
}
void rtiF(CPU *cpuObj){ //#40
// Return from Interrupt
	printf("\nSTACK:\n");
	for (int i=cpuObj->sp; i<=0x1ff; i++){
		printf("%x = %x\n", i, cpuObj->memory->read(i));
	}
	cpuObj->P = cpuObj->memory->read(++cpuObj->sp);
	cpuObj->pc = cpuObj->memory->read(++cpuObj->sp,2);
	cpuObj->sp++;
}
 
void rtsF(CPU *cpuObj){ //#60
// Return from Subroutine
	cpuObj->pc = cpuObj->memory->read(++cpuObj->sp,2);
	cpuObj->sp++;
}

void sbcF(CPU *cpuObj){
// Subtract M from A with Borrow
/* IMMEDIATE:#E9, ZPAGE:#E5, ZPAGEX:#F5, ABSOLUTE:#ED, ABSOLUTEX:#FD
   ABSOLUTEY:#F9, INDIRECTX:#E1, INDIRECTY:#F1 */ 
	int number;
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		number = cpuObj->operand;
	}
	else{
		number = cpuObj->memory->read(cpuObj->operand);
	}
	cpuObj->A -= (number + cpuObj->getPflag(C));
}

void secF(CPU *cpuObj){ //#38	
// Set Carry Flag
	cpuObj->setPflag(C,1);
}

void sedF(CPU *cpuObj){ //#F8
// Set Decimal Mode
	cpuObj->setPflag(D,1);
}

void seiF(CPU *cpuObj){ //#78
// Set Interrupt Disable Status
	cpuObj->setPflag(I,1);
}

void staF(CPU *cpuObj){ 
// Store A in M
/* ZPAGE:#85, ZPAGEX:#95, ABSOLUTE:#8D, ABSOLUTEX:#9D
   ABSOLUTEY:#99, INDIRECTX:#81, INDIRECTY:#91 */ 
	cpuObj->memory->write(cpuObj->operand, cpuObj->A);
}

void stxF(CPU *cpuObj){
// Store X in M
/* ZPAGE:#86, ZPAGEX:#96, ABSOLUTE:#8E */ 
	cpuObj->memory->write(cpuObj->operand, cpuObj->X);
}

void styF(CPU *cpuObj){
// Store Y in M
/* ZPAGE:#84, ZPAGEX:#94, ABSOLUTE:#8C */ 
	cpuObj->memory->write(cpuObj->operand, cpuObj->Y);
}

void taxF(CPU *cpuObj){ //#AA
// Transfer A to X
	cpuObj->X = cpuObj->A;
}

void tayF(CPU *cpuObj){ //#A8
// Transfer A to Y
	cpuObj->Y = cpuObj->A;
}

void tsxF(CPU *cpuObj){ //#BA
// Transfer Stack Pointer to X
	cpuObj->X = cpuObj->sp;
}

void txaF(CPU *cpuObj){ //#8A
// Transfer X to A
	cpuObj->A = cpuObj->X;
}

void txsF(CPU *cpuObj){ //#9A
//Transfer X to Stack Pointer
	cpuObj->sp = cpuObj->X;
}

void tyaF(CPU *cpuObj){ //#98
// Transfer Y to A
	cpuObj->A = cpuObj->Y;
	
}



// Class functions and static variables

/*void CPU::create_mem_map(){

	//
	for (int i=0;i<0x800; i++){
		mem_map[i] = new uint8;
	}

	j = 0;

	for (int i=0x800; i<0x2000; i++){

	}
}*/

CPU::CPU(){
	pc = 0x8000;
	sp = 0x01FF;
	A = 0;
	X = 0;
	Y = 0;
	P = 0;
	operand = 0;
	
}

void CPU::compareElements(uint8 reg){
	uint8 number;
	if(opcodeMode[opcode] == immediate){
		number = operand;
	}
	else{
		number = *memory->map[operand];
	}
	setPflag(C,0);
	if((reg - number)<0){
		setPflag(N,1);
		setPflag(Z,0);
	}
	else if((reg - number) == 0){
		setPflag(N,0);
		setPflag(Z,1);	
	}
	else{
		setPflag(N,0);
		setPflag(Z,0);
	}
}

void CPU::store_value(int addr, uint8 value){
	memory->write(addr,value);
}

uint8 CPU::get_value(int addr){
	return memory->read(addr);
}


void (*jumpTable[256])(CPU *cpuObj) ={
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

int CPU::opcodeCycles[256] ={
	7, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 0, 4, 6, 0, //0
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, //1
	6, 6, 0, 0, 3, 3, 5, 0, 4, 2, 2, 0, 4, 4, 6, 0, //2
};
int CPU::opcodeSize[256] = {

};


	/*

	0  accumulator, 
	1  immediate,	
	2  zpage,
	3  zpageX,
	4  zpageY,
	5  absolute,
	6  absoluteX,
	7  absoluteY,
	8  implied,
	9  relative,
	10 indirectX,
	11 indirectY,
	12 absoluteIndirect
	*/

int CPU::opcodeMode[256] = {
//   0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  A,  B,  C,  D,  E,  F
	 8, 10, 15, 15, 15,  2,  2, 15,  8,  1,  0, 15, 15,  5,  5, 15,  // 0
	 9, 11, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // 1
	 5, 10, 15, 15,  2,  2,  2, 15,  8,  1,  0, 15,  5,  5,  5, 15,  // 2
	 8, 11, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // 3
	 8, 10, 15, 15, 15,  2,  2, 15,  8,  1,  0, 15,  5,  5,  5, 15,  // 4
	 8,  7, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // 5
	 8, 10, 15, 15, 15,  2,  2, 15,  8,  1,  0, 15, 12,  5,  5, 15,  // 6
	 9, 11, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // 7
	15, 10, 15, 15,  2,  2,  2, 15,  8, 15,  8, 15,  5,  5,  5, 15,  // 8
	 9, 11, 15, 15,  3,  3,  4, 15,  8,  7,  8, 15, 15,  6, 15, 15,  // 9
	 1, 10,  1, 15,  2,  2,  2, 15,  8,  1,  8, 15,  5,  5,  5, 15,  // A
	 9, 11, 15, 15,  3,  3,  4, 15,  8,  7,  8, 15,  6,  6,  7, 15,  // B
	 1, 10, 15, 15,  2,  2,  2, 15,  8,  1,  8, 15,  5,  5,  5, 15,  // C
	 9, 11, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // D
	 1, 10, 15, 15,  2,  2,  2, 15,  8,  1,  8, 15,  5,  5,  5, 15,  // E
	 9, 11, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // F
};



uint8 CPU::getPflag(int flagPos){
	uint8 flag = (P >> flagPos) & 0x01;
	return flag;
}

void CPU::setPflag(int flagPos, uint8 value){
	P = (P & ~(0x01 << flagPos)) | (value << flagPos);	
}


int CPU::executeOpcode(){
	jumpTable[opcode](this);
}

void CPU::initialize(Memory *memP){
	memory = memP;
	reset_vector = (memory->read(0xfffd)<<8) | memory->read(0xfffc);
	printf("RESET VECTOR = %x\n", reset_vector);
	reset();
}

void CPU::executeNMI(){
	// pushes the processor status register and return address on the stack
	uint8 pc_high = (cpuObj->pc+2 & 0xFF00) >> 8;
	uint8 pc_low = (cpuObj->pc+2) & 0x00FF;
	cpuObj->memory->write(cpuObj->sp--, pc_high);
	cpuObj->memory->write(cpuObj->sp--, pc_low);
	cpuObj->memory->write(cpuObj->sp--, P)
	pc = (memory->read(0xfffb)<<8) | memory->read(0xfffa);
	//fetchOpcode();
}

void CPU::branch(){

	//uint8 low = (pc & 0x00FF) + (operand & 0x00FF);
	//uint8 high = (pc>>8);//operand>>8);
	sint8 x = (sint8) operand;
	//uint8 low = (pc & 0x00FF) + x;
	printf("BRANCH: pc = %d + (%d) \n",pc,x);
	pc = pc + x;

	//pc = high + low;
}

int CPU::emulateCycles(int cycles=1){
	//int cycle = 0;
	ppustatus = memory->read(0x2002);
	ppuctrl = memory->read(0x2000);
	if((ppustatus&0x80 == 0x80) && (ppuctrl&0x80==0x80)){
		executeNMI();
	}
	while(clock < cycles){
		int ret = fetchOpcode();
	}
	clock = 0;
}

void CPU::reset(){
	//pc = reset_vector;
	pc = (memory->read(0xfffb)<<8) | memory->read(0xfffa);
}

int CPU::fetchOpcode(){

	int mode;
	//int addr = (memory->read(0xFFFF)<<8) | memory->read(0xFFFE);
	opcode = *memory->map[pc];
	mode = opcodeMode[opcode];
	printf("OPCODE = %x \n", opcode);
	printf("pc = %x \n", pc);
	printf("sp = %x \n", sp);
	printf("P = %x \n", P);
	printf("A = %x \n", A);
	printf("banks number = %d \n", memory->cartridge->n_banks);
	printf("0xfffe = %x \n", memory->read(0xfffe));
	printf("0xffff = %x \n", memory->read(0xffff));
	printf("0xfffc = %x \n", memory->read(0xfffc));
	printf("0xfffd = %x \n", memory->read(0xfffd));


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
	int nmode = 1;
	switch(mode){
		case accumulator:
			// Accumulator
			cout << "Mode: accumulator\n";
			operand = -2;
			jumpTable[opcode](this);
			pc+=1;
			break;
		case immediate:
			// Immediate
			cout << "Mode: immediate\n";
			operand = *memory->map[pc+1];
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=2;
			break;
		case zpage:
			// Zero Page
			cout << "Mode: zpage\n";
			operand = *memory->map[pc+1];
			printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=2;
			break;
		case zpageX:
			// Indexed Zero Page with X
			cout << "Mode: zpageX\n";
			operand = *memory->map[pc+1] + X;
			printf("BLIP\n");
			printf("operand = %x\n", operand);
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=2;
			break;
		case zpageY:
			// Indexed Zero Page with Y
			cout << "Mode: zpageY\n";
			operand = *memory->map[pc+1] + Y;
			printf("operand = %x\n", operand);
			jumpTable[opcode](this);
			pc+=2;
			break;
		case absolute:
			// Absolute
		{
			cout << "Mode: absolute\n";
			int less_sig = *memory->map[pc+1];
			int more_sig = *memory->map[pc+2] << 8;
			operand = more_sig | less_sig;
			printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=3;
		}
			break;
		case absoluteX:
			// Indexed Absolute X:
		{
			cout << "Mode: absoluteX\n";
			int less_sig = *memory->map[pc+1];
			int more_sig = *memory->map[pc+2];
			operand = more_sig | less_sig;
			operand += X;
			printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=3;
		}
			break;
		case absoluteY:
			// Indexed Absolute Y	
		{
			cout << "Mode: absoluteY\n";
			int less_sig = *memory->map[pc+1];
			int more_sig = *memory->map[pc+2];
			operand = more_sig | less_sig;
			operand += X;
			printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=3;
		}
			break;	
		case implied:
			// Implied
			cout << "Mode: Implied\n";
			operand = -1;
			jumpTable[opcode](this);
			pc+=1;
			break;
		case relative:
			cout << "Mode: relative\n";
			operand = *memory->map[pc+1];
			printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=2;
			break;
		case indirectX:
			// Pre-indexed indirect, X
		{
			cout << "Mode: indirectX\n";
			int less_sig = *memory->map[*memory->map[pc+1]+X];
			int more_sig = *memory->map[*memory->map[pc+1]+X+1] << 8;
			operand = more_sig | less_sig;
			printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=2;
		}
			break;
		case indirectY:
			// Post-indexed indirect, Y
		{
			cout << "Mode: indirectY\n";
			int less_sig = *memory->map[*memory->map[pc+1]];
			int more_sig = *memory->map[*memory->map[pc+1]+1] << 8;
			operand = more_sig | less_sig;
			operand += Y;
			printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=2;
		}	
			break;
		case absoluteIndirect:
			// Absolute Indirect	
		{
			cout << "Mode: absoluteIndirect\n";
			int pointer = (*memory->map[pc+2] << 8) | *memory->map[pc+1];
			//pointer += 0x8000;
			printf("pointer %x = %x\n", pointer, ((*memory->map[pointer+1] << 8) | *memory->map[pointer]));
			operand = (*memory->map[pointer+1] << 8) | *memory->map[pointer];
			//operand += 0x8000;
			printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			pc+=3;
		}
			break;
		default: 
			nmode = 0;
			cout << "NO MODE!  ";
			cout << "nmode = " << nmode << "\n";
			pc+=1;
			
	}
	cout << "nmode = " << nmode << "\n";
	printf("operand = %x \n",operand);

	for (int i=(sp+1); i<=0x1ff; i++){
		printf("%x = %x\n", i, memory->read(i));
	}
	cout <<"\n";
	return nmode;
}
