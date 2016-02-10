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


void adcF(CPU *cpuObj){
// Add Memory to Accumulator with Carry
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		if((cpuObj->A + cpuObj->operand + cpuObj->C)>0xFF){
			cpuObj->A += cpuObj->operand + cpuObj->C;
		}
	}
	else{
		if((cpuObj->A + cpuObj->memory->read(cpuObj->operand) + cpuObj->C)>0xFF){
			cpuObj->A += cpuObj->memory->read(cpuObj->operand) + cpuObj->C;
		}
	}
}

void andF(CPU *cpuObj){
//  "AND" M with A
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->A &= cpuObj->operand;			
	}
	else{
		cpuObj->A 	&= cpuObj->memory->read(cpuObj->operand);
	}
}

void aslF(CPU *cpuObj){
// Shift Left One Bit (M or A)
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		cpuObj->C = cpuObj->A >> 7;
		cpuObj->A = cpuObj->A << 1;
	}
	else{
		cpuObj->C = cpuObj->memory->read(cpuObj->operand) >> 7;
		*cpuObj->memory->map[cpuObj->operand] = *cpuObj->memory->map[cpuObj->operand] << 1;
	}
}

void bccF(CPU *cpuObj){
// Branch on C Clear
	int operand = cpuObj->operand;
	if(cpuObj->C==0){
		cpuObj->pc += operand;
	}
}

void bcsF(CPU *cpuObj){
// Branch on C Set
	int operand = cpuObj->operand;
	if(cpuObj->C==1){
		cpuObj->pc += operand;
	}
}

void beqF(CPU *cpuObj){
// Branch on Result Zero
	int operand = cpuObj->operand;
	if(cpuObj->Z==1){
		cpuObj->pc += operand;
	}
}

void bitF(CPU *cpuObj){
// Test Bits in M with A
	int operand = cpuObj->operand;
	if((cpuObj->A & *cpuObj->memory->map[operand]) == 0){
		cpuObj->Z = 1;
	}
	else{
		cpuObj->N = *cpuObj->memory->map[operand] >> 7;
		cpuObj->V = (*cpuObj->memory->map[operand] & 0x40) >> 6;
	}
}

void bmiF(CPU *cpuObj){
// Branch on Result Minus
	if(cpuObj->N==1){
		cpuObj->pc += cpuObj->operand;
	}
}

void bneF(CPU *cpuObj){
// Branch on Result not Zero
	if(cpuObj->Z==0){
		cpuObj->pc += cpuObj->operand;
	}
}

void bplF(CPU *cpuObj){
// Branch on Result Plus
	printf("BPL operand %d \n", cpuObj->operand); 
	if(cpuObj->N==0){
		cpuObj->pc += cpuObj->operand;
	}
}

void brkF(CPU *cpuObj){
// Force Break
	*cpuObj->memory->map[cpuObj->sp--] = cpuObj->pc;
	*cpuObj->memory->map[cpuObj->sp--] = cpuObj->P;
}

void bvcF(CPU *cpuObj){
// Branch on Overflow Clear
	if(cpuObj->V==0){
		cpuObj->pc += cpuObj->operand;
	}
}

void bvsF(CPU *cpuObj){
// Branch on Overflow Set
	if(cpuObj->V==1){
		cpuObj->pc += cpuObj->operand;
	}
}

void clcF(CPU *cpuObj){
// Clear C Flag
	cpuObj->C = 0;
}

void cldF(CPU *cpuObj){
// Clear Decimal Mode
	cpuObj->D = 0;
}

void cliF(CPU *cpuObj){
// Clear interrupt Disable Bit
	cpuObj->I = 0;
}

void clvF(CPU *cpuObj){
// Clear Overflow Flag
	cpuObj->V = 0;
}

void cmpF(CPU *cpuObj){
// Compare M and A : A - M 
	cpuObj->compareElements(cpuObj->A);
}

void cpxF(CPU *cpuObj){
// Compare M and X
	cpuObj->compareElements(cpuObj->X);
}

void cpyF(CPU *cpuObj){
// Compare M and Y
	cpuObj->compareElements(cpuObj->Y);
}

void decF(CPU *cpuObj){
// Decrement M by One
	*cpuObj->memory->map[cpuObj->operand] -= 1;
}

void dexF(CPU *cpuObj){
// Decrement X by One
	cpuObj->X -= 1;
}

void deyF(CPU *cpuObj){
//  Decrement Y by One
	cpuObj->Y -= 1;
}

void eorF(CPU *cpuObj){
// "Exclusive-Or" M with A
	int number;
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		number = cpuObj->operand;
	}
	else{
		number = *cpuObj->memory->map[cpuObj->operand];
	}

	cpuObj->A ^= number;
}

void incF(CPU *cpuObj){
// Increment M by One
	*cpuObj->memory->map[cpuObj->operand] += 1;
}

void inxF(CPU *cpuObj){
// Increment X by One
	cpuObj->X += 1;
}

void inyF(CPU *cpuObj){
// Increment Y by One
	cpuObj->Y += 1;
}

void jmpF(CPU *cpuObj){
//  Jump to Location
	cpuObj->pc = *cpuObj->memory->map[cpuObj->operand] - 3;
}

void jsrF(CPU *cpuObj){
// Jump to Location Save Return Address
	*cpuObj->memory->map[cpuObj->sp--] = cpuObj->pc + 3;
	cpuObj->pc = *cpuObj->memory->map[cpuObj->operand] - 3;
}

void ldaF(CPU *cpuObj){
// Load A with M
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->A = cpuObj->operand;
	}
	else{
		cpuObj->A = *cpuObj->memory->map[cpuObj->operand];
	}

}

void ldxF(CPU *cpuObj){
// Load X with M
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->X = cpuObj->operand;
	}
	else{
		cpuObj->X = *cpuObj->memory->map[cpuObj->operand];
	}
}

void ldyF(CPU *cpuObj){
// Load Y with M
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->Y = cpuObj->operand;		
	}
	else{
		cpuObj->Y = *cpuObj->memory->map[cpuObj->operand];
	}
}

void lsrF(CPU *cpuObj){
// Shift Right One Bit (M or A)
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		cpuObj->C = (cpuObj->A & 0x01);
		cpuObj->A = cpuObj->A >> 1;
	}
	else{
		cpuObj->C = (*cpuObj->memory->map[cpuObj->operand] & 0x01);
		*cpuObj->memory->map[cpuObj->operand] = *cpuObj->memory->map[cpuObj->operand] >> 1;
	}
}

void nopF(CPU *cpuObj){
// No Operation
	return;
}

void oraF(CPU *cpuObj){
// "OR" M with A
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->A |= cpuObj->operand;
	}
	else{
		cpuObj->A |= *cpuObj->memory->map[cpuObj->operand];
	}
}


void phaF(CPU *cpuObj){
//Push A on Stack
	*cpuObj->memory->map[cpuObj->sp--] = cpuObj->A;
	//--cpuObj->sp;
}

void phpF(CPU *cpuObj){
// Push Processor Status on Stack
	*cpuObj->memory->map[cpuObj->sp--] = cpuObj->P;
}

void plaF(CPU *cpuObj){
// Pull A from Stack
	cpuObj->A = *cpuObj->memory->map[cpuObj->sp++];
}

void plpF(CPU *cpuObj){
// Pull Processor Status from Stack
	cpuObj->P = *cpuObj->memory->map[cpuObj->sp++];
}

void rolF(CPU *cpuObj){
// Rotate One Bit Left (M or A)
	int aux;
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		aux = (cpuObj->A & 0x80);
		cpuObj->A = cpuObj->A << 1;
		cpuObj->A |= cpuObj->C;				
	}
	else{
		aux = (*cpuObj->memory->map[cpuObj->operand] & 0x80);
		*cpuObj->memory->map[cpuObj->operand] = *cpuObj->memory->map[cpuObj->operand] << 1;
		*cpuObj->memory->map[cpuObj->operand] |= cpuObj->C;		
	}
	cpuObj->C = aux;
}

void rorF(CPU *cpuObj){
// Rotate One Bit Right (M or A)
	int aux;
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		aux = (cpuObj->A & 0x01);
		cpuObj->A = cpuObj->A >> 1;
		cpuObj->A |= (cpuObj->C << 7);
	}
	else{
		aux = (*cpuObj->memory->map[cpuObj->operand] & 0x01);
		*cpuObj->memory->map[cpuObj->operand] = *cpuObj->memory->map[cpuObj->operand] >> 1;
		*cpuObj->memory->map[cpuObj->operand] |= (cpuObj->C << 7);
	}
	cpuObj->C = aux;
}
void rtiF(CPU *cpuObj){
// Return from Interrupt
	cpuObj->P = *cpuObj->memory->map[cpuObj->sp++];
	cpuObj->pc = *cpuObj->memory->map[cpuObj->sp++];
}

void rtsF(CPU *cpuObj){
// Return from Subroutine
	cpuObj->pc = *cpuObj->memory->map[cpuObj->sp++];
}

void sbcF(CPU *cpuObj){
// Subtract M from A with Borrow
	int number;
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		number = cpuObj->operand;
	}
	else{
		number = *cpuObj->memory->map[cpuObj->operand];
	}
	cpuObj->A -= (number + cpuObj->C);
}

void secF(CPU *cpuObj){
// Set Carry Flag
	cpuObj->C = 1;
}

void sedF(CPU *cpuObj){
// Set Decimal Mode
	cpuObj->D = 1;
}

void seiF(CPU *cpuObj){
// Set Interrupt Disable Status
	cpuObj->I = 1;
}

void staF(CPU *cpuObj){
// Store A in M
	*cpuObj->memory->map[cpuObj->operand] = cpuObj->A;
}

void stxF(CPU *cpuObj){
// Store X in M
	*cpuObj->memory->map[cpuObj->operand] = cpuObj->X;
}

void styF(CPU *cpuObj){
// Store Y in M
	*cpuObj->memory->map[cpuObj->operand] = cpuObj->Y;
}

void taxF(CPU *cpuObj){
// Transfer A to X
	cpuObj->X = cpuObj->A;
}

void tayF(CPU *cpuObj){
// Transfer A to Y
	cpuObj->Y = cpuObj->A;
}

void tsxF(CPU *cpuObj){
// Transfer Stack Pointer to X
	cpuObj->X = cpuObj->sp;
}

void txaF(CPU *cpuObj){
// Transfer X to A
	cpuObj->A = cpuObj->X;
}

void txsF(CPU *cpuObj){
//Transfer X to Stack Pointer
	cpuObj->sp = cpuObj->X;
}

void tyaF(CPU *cpuObj){
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
	pc = 0xC000;
	sp = 0;
	A = 0;
	X = 0;
	Y = 0;
	P = 0;
	N = 0;
	Z = 0;
	C = 0;
	I = 0;
	D = 0;
	V = 0;
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
	 8, 10, 15, 15,  2,  2,  2, 15,  8,  1,  0, 15,  5,  5,  5, 15,  // 2
	 8, 11, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // 3
	 8, 10, 15, 15, 15,  2,  2, 15,  8,  1,  0, 15,  5,  5,  5, 15,  // 4
	 8,  7, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // 5
	 8, 10, 15, 15, 15,  2,  2, 15,  8,  1,  0, 15, 12,  5,  5, 15,  // 6
	 8, 11, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // 7
	15, 10, 15, 15,  2,  2,  2, 15,  8, 15,  8, 15,  5,  5,  5, 15,  // 8
	 8, 11, 15, 15,  3,  3,  4, 15,  8,  7,  8, 15, 15,  6, 15, 15,  // 9
	 1, 10,  1, 15,  2,  2,  2, 15,  8,  1,  8, 15,  5,  5,  5, 15,  // A
	 8, 11, 15, 15,  3,  3,  4, 15,  8,  7,  8, 15,  6,  6,  7, 15,  // B
	 1, 10, 15, 15,  2,  2,  2, 15,  8,  1,  8, 15,  5,  5,  5, 15,  // C
	 9, 11, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // D
	 1, 10, 15, 15,  2,  2,  2, 15,  8,  1,  8, 15,  5,  5,  5, 15,  // E
	 8, 11, 15, 15, 15,  3,  3, 15,  8,  7, 15, 15, 15,  6,  6, 15,  // F
};

int CPU::executeOpcode(){
	jumpTable[opcode](this);
}

void CPU::initialize(Memory *memP){
	memory = memP;
}

int CPU::emulateCycle(){

	int ret = fetchOpcode();
	return ret;
}

int CPU::fetchOpcode(){

	int mode;
	opcode = *memory->map[pc];
	mode = opcodeMode[opcode];
	printf("OPCODE = %x \n", opcode);
	printf("pc = %x \n", pc);

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
			pc+=2;
			break;
		case immediate:
			// Immediate
			cout << "Mode: immediate\n";
			operand = *memory->map[pc+1];
			jumpTable[opcode](this);
			pc+=2;
			break;
		case zpage:
			// Zero Page
			cout << "Mode: zpage\n";
			operand = *memory->map[pc+1];
			jumpTable[opcode](this);
			pc+=2;
			break;
		case zpageX:
			// Indexed Zero Page with X
			cout << "Mode: zpageX\n";
			operand = *memory->map[pc+1] + X;
			jumpTable[opcode](this);
			pc+=2;
			break;
		case zpageY:
			// Indexed Zero Page with Y
			cout << "Mode: zpageY\n";
			operand = *memory->map[pc+1] + Y;
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
			jumpTable[opcode](this);
			pc+=2;
		}	
			break;
		case absoluteIndirect:
			// Absolute Indirect	
		{
			cout << "Mode: absoluteIndirect\n";
			int pointer = (*memory->map[pc+2] << 8) | *memory->map[pc+1];
			operand = (*memory->map[pointer+1] << 8) | *memory->map[pointer];
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
	cout <<"\n";
	return nmode;
}
