#include "CPU.h"
#include "cpu_tests.h"
#define PRG_OFFSET 0x8000
//#define DEBUG 

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

long int inst_counter = 0;
/*  M = memory
   	A = A register ( Accumulator)
	X = X register
	Y = Y register
*/


char ascii_flag_map[8] = {
	99,
	122,
	105,
	100,
	98,
	117,
	118,
	110
};


bool pageCrossed(uint8 addr1, uint8 addr2){
	return ((addr1&0xFF00)!=(addr2&0xFF00));
}

/* ALFABETICAL ORDER */


int adcF(CPU *cpuObj){ //
// Add Memory to Accumulator with Carry
	//uint8 carry = cpuObj->getPflag(C)
	uint16 res;
	uint8 operand;
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){ //#69
		operand = cpuObj->operand;
		//res = cpuObj->sum_operation(cpuObj->A, cpuObj->operand, cpuObj->status[C]);
		/*res = cpuObj->A + cpuObj->operand + cpuObj->status[C];
		res = cpuObj->comp2Operation(cpuObj->A, cpuObj->operand, true);;*/

	}
	else{
	/* ZPAGE:#65, ZPAGEX:#75, ABSOLUTE:#6D, ABSOLUTEX:#7D
	   ABSOLUTEY:#79, INDIRECTX:#61, INDIRECTY:#71
	*/
	   	operand = cpuObj->memory->read(cpuObj->operand);
	   //res = cpuObj->sum_operation(cpuObj->A, cpuObj->memory->read(cpuObj->operand), cpuObj->status[C]);
	    //res = cpuObj->comp2Operation(cpuObj->A, cpuObj->memory->read(cpuObj->operand), '+', true);
	}
	res = cpuObj->sum_operation(cpuObj->A, operand, cpuObj->status[C]);
	cpuObj->setSignalFlags(res);
	cpuObj->check_overflow(cpuObj->A, operand,res);
	cpuObj->A = res;
	return 0;
}

int andF(CPU *cpuObj){
//  "AND" M with A
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){ //#29
		cpuObj->A &= cpuObj->operand;
	}
	else{
		/* ZPAGE:#25, ZPAGEX:#35, ABSOLUTE:#2D, ABSOLUTEX:#3D
		   ABSOLUTEY:#39, INDIRECTX:#21, INDIRECTY:#31 */
		cpuObj->A 	&= cpuObj->memory->read(cpuObj->operand);
	}
	cpuObj->setSignalFlags(cpuObj->A);
	return 0;
}

int aslF(CPU *cpuObj){
// Shift Left One Bit (M or A)
	uint8 aux;
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){ //#0A
		//cpuObj->setPflag(C,cpuObj->A >> 7);
		cpuObj->status[C] = (cpuObj->A&0x80)>>7;
		cpuObj->A = cpuObj->A << 1;
		aux = cpuObj->A;
	}
	else{
		//uint8 aux;
		/* ZPAGE:#06, ZPAGEX:#16, ABSOLUTE:#0E, ABSOULTEX:#1E */
		//cpuObj->setPflag(C, cpuObj->memory->read(cpuObj->operand) >> 7);
		aux = cpuObj->memory->read(cpuObj->operand);
		cpuObj->status[C] = (aux&0x80)>>7;
		aux = aux<<1;
		cpuObj->memory->write(cpuObj->operand, aux);
	}
	cpuObj->setSignalFlags(aux);
	return 0;
}

int bccF(CPU *cpuObj){ //#90
// Branch on C Clear
	//int operand = cpuObj->operand;
	if(cpuObj->status[C]==0){
		return cpuObj->branch();
	}
	return 0;
}

int bcsF(CPU *cpuObj){ //#B0
// Branch on C Set
	//int operand = cpuObj->operand;
	if(cpuObj->status[C]==1){
		return cpuObj->branch();
	}
	return 0;
}


int beqF(CPU *cpuObj){//#F0
// Branch on Result Zero
	if(cpuObj->status[Z]==1){
		return cpuObj->branch();
	}
	return 0;
}

int bitF(CPU *cpuObj){
// Test Bits in M with A
	/* ZPAGE:#24, ABSOLUTE:#2C */
	// TODO: is this correct?
	uint8 memV = cpuObj->memory->read(cpuObj->operand);
	uint8 aux = cpuObj->A & memV;
	//setSignalFlags(aux);
	if(aux == 0){
		cpuObj->status[Z]=1;
	}
	else{
		cpuObj->status[Z]=0;
	}

	//printf("BIT memV is %x, N should be %x\n",memV, (memV>>7));
	cpuObj->status[N] = memV>>7;
	//printf("BIT memV is %x, N should be %x\n",memV, (memV>>7));
	cpuObj->status[V] = (memV&0x40)>>6;

	return 0;
}

int bmiF(CPU *cpuObj){ //#30
// Branch on Result Minus
	//printf("\n\nBMI \n\n");
	if(cpuObj->status[N]==1){
		return cpuObj->branch();
	}
	return 0;
}

int bneF(CPU *cpuObj){ //#D0
// Branch on Result not Zero
	if(cpuObj->status[Z]==0){
		return cpuObj->branch();
	}
	return 0;
}

int bplF(CPU *cpuObj){ //#10
// Branch on Result Plus
	//printf("BPL operand %d \n", cpuObj->operand);
	if(cpuObj->status[N]==0){
		return cpuObj->branch();
	}
	return 0;
}



int brkF(CPU *cpuObj){ //#00
// Force Break
	//cpuObj->memory->write(cpuObj->sp--, cpuObj->pc);
	//cpuObj->pc += 1;
	uint8 pc_high = ((cpuObj->pc+2) & 0xFF00) >> 8;
	uint8 pc_low = (cpuObj->pc+2) & 0x00FF;
	cpuObj->memory->write(cpuObj->sp--, pc_high);
	cpuObj->memory->write(cpuObj->sp--, pc_low);
	cpuObj->push_status_to_Stack(false);//memory->write(cpuObj->sp--, cpuObj->P);
	cpuObj->pc = ((cpuObj->memory->read(0xffff)<<8) | cpuObj->memory->read(0xfffe)) -1;
	cpuObj->status[I] = 1;
	//cpuObj->pc = cpuObj->memory->read(cpuObj->operand) + PRG_ADDR - 3;
	return 0;
}

int bvcF(CPU *cpuObj){ //#50
// Branch on Overflow Clear
	if(cpuObj->status[V]==0){
		return cpuObj->branch();
	}
	return 0;
}

int bvsF(CPU *cpuObj){ //#70
// Branch on Overflow Set
	if(cpuObj->status[V]==1){
		return cpuObj->branch();
	}
	return 0;
}

int clcF(CPU *cpuObj){ //#18
// Clear C Flag
	cpuObj->status[C]=0;
	return 0;
}

int cldF(CPU *cpuObj){ //#D8
// Clear Decimal Mode
	cpuObj->status[D]=0;
	return 0;
}

int cliF(CPU *cpuObj){ //#58
// Clear interrupt Disable Bit
	cpuObj->status[I]=0;
	return 0;
}

int clvF(CPU *cpuObj){ //#B8
// Clear Overflow Flag
	cpuObj->status[V]=0;
	return 0;
}

int cmpF(CPU *cpuObj){
// Compare M and A : A - M
/* IMMEDIATE:#C9, ZPAGE:#C5, ZPAGEX:#D5, ABSOLUTE:#CD, ABSOLUTEX:#DD
   ABSOLUTEY:#D9, INDIRECTX:#C1, INDIRECTY:#D1 */
	cpuObj->compareElements(cpuObj->A);
	return 0;
}

int cpxF(CPU *cpuObj){
// Compare M and X
/* IMMEDIATE:#E0, ZPAGE:#E4, ABSOLUTE:#EC */
	cpuObj->compareElements(cpuObj->X);
	return 0;
}

int cpyF(CPU *cpuObj){
// Compare M and Y
/* IMMEDIATE:#C0, ZPAGE:#C4, ABSOLUTE:#CC */
	cpuObj->compareElements(cpuObj->Y);
	return 0;
}

int decF(CPU *cpuObj){
// Decrement M by One
/* ZPAGE:#C6, ZPAGEX:#D6, ABSOLUTE:#CE, ABSOLUTEX:#DE */
	uint8 memV = cpuObj->memory->read(cpuObj->operand);

	if(memV-1==0){
		cpuObj->status[Z]=1;
	}
	else{
		cpuObj->status[Z]=0;
	}
	cpuObj->memory->write(cpuObj->operand, memV - 1);
	cpuObj->status[N] = (memV-1)>>7;
	return 0;

}

int dexF(CPU *cpuObj){ //#CA
// Decrement X by One
	cpuObj->X -= 1;
	if(cpuObj->X==0){
		cpuObj->status[Z]=1;
	}
	else{
		cpuObj->status[Z]=0;
	}
	cpuObj->status[N] = (cpuObj->X)>>7;
	return 0;

}

int deyF(CPU *cpuObj){ //#88
//  Decrement Y by One
	cpuObj->Y -= 1;
	if(cpuObj->Y==0){
		cpuObj->status[Z]=1;
	}
	else{
		cpuObj->status[Z]=0;
	}
	cpuObj->status[N] = (cpuObj->Y)>>7;
	return 0;
}

int eorF(CPU *cpuObj){
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
	cpuObj->setSignalFlags(cpuObj->A);
	return 0;
}

int incF(CPU *cpuObj){
// Increment M by One
/* ZPAGE:#E6, ZPAGEX:#F6, ABSOLUTE:#EE, ABSOLUTEX:#FE */
	uint8 memV = cpuObj->memory->read(cpuObj->operand);
	cpuObj->memory->write(cpuObj->operand, memV + 1);
	cpuObj->setSignalFlags(memV+1);
	return 0;
}

int inxF(CPU *cpuObj){ //#E8
// Increment X by One
	cpuObj->X += 1;
	cpuObj->setSignalFlags(cpuObj->X);
	return 0;
}

int inyF(CPU *cpuObj){ //#C8
// Increment Y by One
	cpuObj->Y += 1;
	cpuObj->setSignalFlags(cpuObj->Y);
	return 0;
}

int jmpF(CPU *cpuObj){
//  Jump to Location
/* INDIRECT:#6C, ABSOLUTE:#4C */

	cpuObj->pc =  cpuObj->operand - 3;
	return 0;

}

int jsrF(CPU *cpuObj){ //#20
// Jump to Location Save Return Address
	uint8 pc_high = ((cpuObj->pc+2) & 0xFF00) >> 8;
	uint8 pc_low = (cpuObj->pc+2) & 0x00FF;
	cpuObj->memory->write(cpuObj->sp--, pc_high);
	cpuObj->memory->write(cpuObj->sp--, pc_low);
	cpuObj->pc = cpuObj->operand - 3;
	return 0;
	/*for (int i=cpuObj->sp; i<=0x1ff; i++){
		printf("%x = %x\n", i, cpuObj->memory->read(i));
	}*/
}

int ldaF(CPU *cpuObj){
// Load A with M
/* IMMEDIATE:#A9, ZPAGE:#A5, ZPAGEX:#B5, ABSOLUTE:#AD, ABSOLUTEX:#BD
   ABSOLUTEY:#B9, INDIRECTX:#A1, INDIRECTY:#B1 */
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		//printf("cpuOBJ->A before = %x\n", cpuObj->A);
		cpuObj->A = (short) cpuObj->operand;
		//printf("operandeeee = %x\n", cpuObj->operand);
		//printf("cpuOBJ->A = %x\n", cpuObj->A);
	}
	else{

		cpuObj->A = cpuObj->memory->read(cpuObj->operand);
		//printf("MEM TO A = %x",cpuObj->A);

	}
	cpuObj->setSignalFlags(cpuObj->A);
	return 0;

}

int ldxF(CPU *cpuObj){
// Load X with M
/* IMMEDIATE:#A2, ZPAGE:#A6, ZPAGEY:#B6, ABSOLUTE:#AE, ABSOLUTEY:#BE */
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->X = cpuObj->operand;
	}
	else{
		cpuObj->X = cpuObj->memory->read(cpuObj->operand);
	}
	cpuObj->setSignalFlags(cpuObj->X);
	return 0;
}

int ldyF(CPU *cpuObj){
// Load Y with M
/* IMMEDIATE:#A0, ZPAGE:#A4, ZPAGEX:#B4, ABSOLUTE:#AC, ABSOLUTEX:#BC */
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->Y = cpuObj->operand;
	}
	else{
		cpuObj->Y = cpuObj->memory->read(cpuObj->operand);
	}
	cpuObj->setSignalFlags(cpuObj->Y);
	return 0;
}

int lsrF(CPU *cpuObj){
// Shift Right One Bit (M or A)
/* ACCUMULATOR:#4A, ZPAGE:#46, ZPAGEX:#56, ABSOLUTE:#4E, ABSOLUTEX:#5E */
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		//cpuObj->setPflag(C,(cpuObj->A & 0x01));
		cpuObj->status[C] = cpuObj->A & 0x01;
		cpuObj->A = cpuObj->A >> 1;
		cpuObj->setSignalFlags(cpuObj->A);
		//printf("LSR ACCUMULATOR!!");
	}
	else{
		uint8 memV = cpuObj->memory->read(cpuObj->operand);
		//cpuObj->setPflag(C,(memV & 0x01));
		cpuObj->status[C] = memV & 0x01;
		memV = memV >> 1;
		cpuObj->memory->write(cpuObj->operand, memV);
		cpuObj->setSignalFlags(memV);
		//printf("LSR MEMORY!");
	}
	return 0;
}

int nopF(CPU *cpuObj){ //#EA
// No Operation
	return 0;
}

int oraF(CPU *cpuObj){
// "OR" M with A
/* IMMEDIATE:#09, ZPAGE:#05, ZPAGEX:#15, ABSOLUTE:#0D, ABSOLUTEX:#1D
   ABSOLUTEY:#19, INDIRECTX:#01, INDIRECTY:#11 */
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		cpuObj->A |= cpuObj->operand;
	}
	else{
		cpuObj->A |= cpuObj->memory->read(cpuObj->operand);
	}
	cpuObj->setSignalFlags(cpuObj->A);
	return 0;
}


int phaF(CPU *cpuObj){ //#48
//Push A on Stack
	cpuObj->memory->write(cpuObj->sp, cpuObj->A);
	cpuObj->updateSP(0);
	return 0;

	//--cpuObj->sp;
}

int phpF(CPU *cpuObj){ //#08
// Push Processor Status on Stack
	cpuObj->push_status_to_Stack(false);//memory->write(cpuObj->sp--, cpuObj->P);
	return 0;
}

int plaF(CPU *cpuObj){ //#68
// Pull A from Stack
	cpuObj->updateSP(1);
	cpuObj->A = cpuObj->memory->read(cpuObj->sp);
	cpuObj->setSignalFlags(cpuObj->A);
	return 0;
}

int plpF(CPU *cpuObj)
{ //#28
// Pull Processor Status from Stack
	//cpuObj->P
	cpuObj->pop_status_from_Stack();
	/*uint8 p = cpuObj->memory->read(++cpuObj->sp);// <NV-BDIZC>
	for (int i=0;i<8;i++){
		if(i==5){
			continue;
		}
		cpuObj->status[i] = (p>>i)&0x01;
	}*/
	return 0;
}

int rolF(CPU *cpuObj){
// Rotate One Bit Left (M or A)
/* ACCUMULATOR:#2A, ZPAGE:#26, ZPAGEX:#36, ABSOLUTE:#2E, ABSOLUTEX:#3E */
	int aux;
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		aux = (cpuObj->A & 0x80);
		cpuObj->A = cpuObj->A << 1;
		cpuObj->A |= cpuObj->status[C];
		cpuObj->setSignalFlags(cpuObj->A);

	}
	else{
		uint8 memV = cpuObj->memory->read(cpuObj->operand);
		int n = memV << 1 | cpuObj->status[C];
		aux = (memV & 0x80);
		cpuObj->memory->write(cpuObj->operand, n);
		cpuObj->setSignalFlags(n);
		//*cpuObj->memory->write(cpuObj->operand] = memV << 1;
		//*cpuObj->memory->map[cpuObj->operand] |= cpuObj->C;
	}
	//cpuObj->setPflag(C,aux);
	cpuObj->status[C] = aux>>7;
	return 0;

}

int rorF(CPU *cpuObj){
// Rotate One Bit Right (M or A)
/* ACCUMULATOR:#6A, ZPAGE:#66, ZPAGEX:#76, ABSOLUTE:#6E, ABSOLUTEX:#7E */
	int aux;
	if(CPU::opcodeMode[cpuObj->opcode] == accumulator){
		aux = (cpuObj->A & 0x01);
		cpuObj->A = cpuObj->A >> 1;
		cpuObj->A |= (cpuObj->status[C]<< 7);
		cpuObj->setSignalFlags(cpuObj->A);
	}
	else{
		int memV = cpuObj->memory->read(cpuObj->operand);
		int n = (cpuObj->status[C]<< 7) | (memV >> 1);
		aux = (memV & 0x01);
		cpuObj->memory->write(cpuObj->operand, n);
		cpuObj->setSignalFlags(n);
		//*cpuObj->memory->map[cpuObj->operand] |= (cpuObj->C << 7);
	}
	cpuObj->status[C] = aux;
	return 0;
}
int rtiF(CPU *cpuObj){ //#40
// Return from Interrupt
	//uint8 P;
	/*printf("\nSTACK:\n");
	for (int i=cpuObj->sp; i<=0x1ff; i++){
		printf("%x = %x\n", i, cpuObj->memory->read(i));
	}*/
	//P = cpuObj->memory->read(++cpuObj->sp);
	//for(int i=0;i<8;i++){
	cpuObj->pop_status_from_Stack();
	cpuObj->pc = cpuObj->memory->read(++cpuObj->sp,2)-1;
	cpuObj->sp++;
	//if (cpuObj->on_nmi == False
	return 0;
}

int rtsF(CPU *cpuObj){ //#60
// Return from Subroutine
	cpuObj->pc = cpuObj->memory->read(++cpuObj->sp,2);
	//printf("rts pc = %x", cpuObj->pc);
	cpuObj->sp++;
	return 0;
}

int sbcF(CPU *cpuObj){
// Subtract M from A with Borrow
/* IMMEDIATE:#E9, ZPAGE:#E5, ZPAGEX:#F5, ABSOLUTE:#ED, ABSOLUTEX:#FD
   ABSOLUTEY:#F9, INDIRECTX:#E1, INDIRECTY:#F1 */
	uint8 number;
	if(CPU::opcodeMode[cpuObj->opcode] == immediate){
		number = cpuObj->operand;
	}
	else{
		number = cpuObj->memory->read(cpuObj->operand);
	}

	uint8 res = cpuObj->sub_operation(cpuObj->A,number,cpuObj->status[C]);	
	//uint8 res = cpuObj->comp2Operation(cpuObj->A,number,'-',true);
	cpuObj->setSignalFlags(res);
	cpuObj->check_overflow(cpuObj->A, ~number,res);
	cpuObj->A = res;
	return 0;
}

int secF(CPU *cpuObj){ //#38
// Set Carry Flag
	cpuObj->status[C]=1;
	return 0;
}

int sedF(CPU *cpuObj){ //#F8
// Set Decimal Mode
	cpuObj->status[D]=1;
	return 0;
}

int seiF(CPU *cpuObj){ //#78
// Set Interrupt Disable Status
	cpuObj->status[I]=1;
	return 0;
}

int staF(CPU *cpuObj){
// Store A in M
/* ZPAGE:#85, ZPAGEX:#95, ABSOLUTE:#8D, ABSOLUTEX:#9D
   ABSOLUTEY:#99, INDIRECTX:#81, INDIRECTY:#91 */
	cpuObj->memory->write(cpuObj->operand, cpuObj->A);
	return 0;
}

int stxF(CPU *cpuObj){
// Store X in M
/* ZPAGE:#86, ZPAGEX:#96, ABSOLUTE:#8E */
	cpuObj->memory->write(cpuObj->operand, cpuObj->X);
	return 0;
}

int styF(CPU *cpuObj){
// Store Y in M
/* ZPAGE:#84, ZPAGEX:#94, ABSOLUTE:#8C */
	cpuObj->memory->write(cpuObj->operand, cpuObj->Y);
	return 0;
}

int taxF(CPU *cpuObj){ //#AA
// Transfer A to X
	cpuObj->X = cpuObj->A;
	cpuObj->setSignalFlags(cpuObj->X);
	return 0;
}

int tayF(CPU *cpuObj){ //#A8
// Transfer A to Y
	cpuObj->Y = cpuObj->A;
	cpuObj->setSignalFlags(cpuObj->Y);
	return 0;
}

int tsxF(CPU *cpuObj){ //#BA
// Transfer Stack Pointer to X
	cpuObj->X = cpuObj->sp;
	cpuObj->setSignalFlags(cpuObj->X);
	return 0;
}

int txaF(CPU *cpuObj){ //#8A
// Transfer X to A
	cpuObj->A = cpuObj->X;
	cpuObj->setSignalFlags(cpuObj->A);
	return 0;
}

int txsF(CPU *cpuObj){ //#9A
//Transfer X to Stack Pointer
	cpuObj->sp = 0x100 + cpuObj->X;
	//cpuObj->setSignalFlags(cpuObj->A);
	return 0;
}

int tyaF(CPU *cpuObj){ //#98
// Transfer Y to A
	cpuObj->A = cpuObj->Y;
	cpuObj->setSignalFlags(cpuObj->A);
	return 0;

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
	char schar[8] = {'c','z','i','d','b','u','v','n'};//{'n','v','u','b','d','i','z','c'};
	pc = 0x8000;
	sp = 0x01FD;
	A = 0;
	X = 0;
	Y = 0;
	P = 0;
	addr_brk = 0;
	operand = 0;
	for(int i=0; i<8; i++){
		status[i]=0;
	}
	//status[U] = 1;
	//status[B] = 1;


	//FOR DEBUG
	for (int i=0; i<8; i++){
		stat_chart[i] = schar[i];
	}
	inst_counter = 0;


}

inline void CPU::updateSP(int increase){
	/* Stack pointer should wrap around if goes out of range 1FF-100 */
	if (increase){
		sp++;
	}
	else {
		sp--;
	}
	sp = (sp&0x01ff) | 0x0100;
}

uint8 CPU::check_overflow (uint8 n1, uint8 n2, uint8 res){
	uint8 vflag = 0;
	uint16 sres;
	/*
		http://www.6502.org/tutorials/vflag.html
		http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
	*/	
	// Cheating here to set overflow flag :P
	/*sres = (sint8)n1 + (sint8)n2 + (sint8)c;

	if ((sres > 127)||(sres<-128)){
		V = 1;
	}*/
	if ((((n1^n2)&0x80)==0) && (((n1^res)&0x80)==0x80)){
		vflag = 1;
	}
	status[V] = vflag;
	//printf("Checking overflow for %x + %x = %x\n",n1,n2,res);
	//printf("status[V] = %u, V = %u\n", status[V],vflag);

	return vflag;


	/* Overflow condition on different sign cases

	n1 | n2 | c | V condition
	 + | +  | + | n1 + n2 + c > 0x7f (127 decimal)
	 + | +  | - | (n1!=0 and n2!=0) && (n1 + n2 + c > 0x7f (127 decimal))
	 + | -  | + | never
	 + | -  | - | n1 + n2 + c > 0xff (-128 decimal)
	 - | +  | + | never
	 - | +  | - | n1 + n2 + c > 0xff (-128 decimal)
	 - | -  | + | n1 + n2 + c > 0xff (-128 decimal)
	 - | -  | - | n1 + n2 + c > 0xff (-128 decimal)
	 

	// Check if n1 and n2 have same signals.
	if (((n1^n2)&0x80)==0) { 
		//printf("Same signals \n");
		// Check negative number overflow
		if ((n1&0x80)==0x80) {
			if ((n1+n2+c)>0xff)
				V = 1;		
		}
		else {
			// Check positive number overflow
			if ((n1!=0)&&(n2!=0)&&(n1+n2+c)>0x7f)
				V = 1;
		}
	}
	else if ((n1+n2+c)>0xff){
		// For different signals it will only overflow on negative results
		V = 1;
	}/*

	return V;*/
}

uint8 CPU::sum_operation(uint8 n1, uint8 n2, uint8 carry){
	uint8 res;

	//status[V] = check_overflow(n1,n2,carry);

	if ((n1+n2+carry)>0xff){
		status[C] = 1;
	}
	else {
		status[C] = 0;
	}

	res = n1 + n2 + carry;
	return res;
}



uint8 CPU::sub_operation(uint8 n1, uint8 n2, uint8 carry){
	uint8 v1, res,c1;
	/* For subtraction we just need to invert the bits of n2 and add them
	   together with n1 and the carry (inverted borrow)
	   Subtraction operations should always start with the carry flag set
	   it is the ROM's job to set that flag before starting a subtraction.
	*/
	n2 = ~n2; 

	res = sum_operation(n1,n2,carry);
	return res;
}
//complement of 2 operations
uint8 CPU::comp2Operation(uint8 n1, uint8 n2, char op, bool withcarry){
	uint16 res16;
	uint8 result, partial;
	uint8 V1;
	uint16 c = status[C];

	//printf("n1 = %x, n2 = %x\n",n1,n2);

	if (op == '-'){
		//printf("~n2 = %x\n",~n2);
		n2  = ~n2 + 1;
		//printf("-n2 = %x\n",n2);
		c = ~c + 1;
		//printf("-c = %x\n",c);
	}
	if (withcarry){

		partial = n1 + n2;
		res16 = n1 + n2; //+ c;
		result = (uint8) res16;

		// Overflow flag
		// http://www.6502.org/tutorials/vflag.html
		if ((((n1^n2)&0x80)==0) && (((n1^partial)&0x80)==0x80)){
			V1 = 1;
		}
		else{
			V1 = 0;
		}
		if ((((partial^c)&0x80)==0) && (((partial^res16)&0x80)==0x80)){
			status[V] = (V1 | 0x01);
		}
		else{
			status[V] = (V1 | 0x00);
		}
	}
	else {
		res16 = n1 + n2;
		result = (uint8) res16;
		// Overflow flag
		// http://www.6502.org/tutorials/vflag.html
		if (((n1^n2)&0x80)==0 && ((n1^result)&0x80)==0x80){
			status[V] = 1;
		}
		else{
			status[V] = 0;
		}
	}
	// Carry flag
	//
	if (res16>0xFF){
		status[C] = 1;
	}
	else{
		status[C] = 0;
	}

	// Set N and Z
	//printf("n1 = %x, n2 = %x\n",n1,n2);
	//printf("res16 = %x, result = %x\n",res16,result);
	setSignalFlags(result);
	return result;
}



void CPU::compareElements(uint8 reg){
	//TODO: SET CARRY FLAG, WHEN SHOULD IT BE SET HERE?
	uint8 number;
	if(opcodeMode[opcode] == immediate){
		number = operand;
	}
	else{
		number = *memory->map[operand];
	}
	uint8 res = sub_operation(reg,number,1);
	setSignalFlags(res);

	//printf("compare res = %x\n", res);
	//comp2Operation(reg,number,'-');
	//setSignalFlags(res);
	//printf("COMP RES = %x\n",res);

	/*setPflag(C,0);
	if((reg - number)<0){
		status[N]=1;
		status[Z]=0;
	}
	else if((reg - number) == 0){
		status[N]=0;
		status[Z]=1;
	}
	else{
		status[N]=0;
		status[Z]=0;
	}*/
}

void CPU::store_value(int addr, uint8 value){
	memory->write(addr,value);
}

uint8 CPU::get_value(int addr){
	return memory->read(addr);
}


int (*jumpTable[256])(CPU *cpuObj) ={
	brkF, oraF, nopF, nopF, nopF, oraF, aslF, nopF, phpF, oraF, aslF, nopF, nopF, oraF, aslF, nopF, //0
	bplF, oraF, nopF, nopF, nopF, oraF, aslF, nopF, clcF, oraF, nopF, nopF, nopF, oraF, aslF, nopF, //1
	jsrF, andF, nopF, nopF, bitF, andF, rolF, nopF, plpF, andF, rolF, nopF, bitF, andF, rolF, nopF, //2
	bmiF, andF, nopF, nopF, nopF, andF, rolF, nopF, secF, andF, nopF, nopF, nopF, andF, rolF, nopF, //3
	rtiF, eorF, nopF, nopF, nopF, eorF, lsrF, nopF, phaF, eorF, lsrF, nopF, jmpF, eorF, lsrF, nopF, //4
	bvcF, eorF, nopF, nopF, nopF, eorF, lsrF, nopF, cliF, eorF, nopF, nopF, nopF, eorF, lsrF, nopF, //5
	rtsF, adcF, nopF, nopF, nopF, adcF, rorF, nopF, plaF, adcF, rorF, nopF, jmpF, adcF, rorF, nopF, //6
	bvsF, adcF, nopF, nopF, nopF, adcF, rorF, nopF, seiF, adcF, nopF, nopF, nopF, adcF, rorF, nopF, //7
	nopF, staF, nopF, nopF, styF, staF, stxF, nopF, deyF, nopF, txaF, nopF, styF, staF, stxF, nopF, //8
	bccF, staF, nopF, nopF, styF, staF, stxF, nopF, tyaF, staF, txsF, nopF, nopF, staF, nopF, nopF, //9
	ldyF, ldaF, ldxF, nopF, ldyF, ldaF, ldxF, nopF, tayF, ldaF, taxF, nopF, ldyF, ldaF, ldxF, nopF, //A
	bcsF, ldaF, nopF, nopF, ldyF, ldaF, ldxF, nopF, clvF, ldaF, tsxF, nopF, ldyF, ldaF, ldxF, nopF, //B
	cpyF, cmpF, nopF, nopF, cpyF, cmpF, decF, nopF, inyF, cmpF, dexF, nopF, cpyF, cmpF, decF, nopF, //C
	bneF, cmpF, nopF, nopF, nopF, cmpF, decF, nopF, cldF, cmpF, nopF, nopF, nopF, cmpF, decF, nopF, //D
	cpxF, sbcF, nopF, nopF, cpxF, sbcF, incF, nopF, inxF, sbcF, nopF, nopF, cpxF, sbcF, incF, nopF, //E
	beqF, sbcF, nopF, nopF, nopF, sbcF, incF, nopF, sedF, sbcF, nopF, nopF, nopF, sbcF, incF, nopF  //F
	//0 ,  1  ,  2  ,  3  ,  4  ,  5  ,  6  ,  7  ,  8  ,  9  ,  A  ,  B  ,  C  ,  D  ,  E  ,  F

};

std::string CPU::opcodeNameTable[256] = 
{
	"BRK", "ORA", "NOP", "NOP", "NOP", "ORA", "ASL", "NOP", "PHP", "ORA", "ASL", "NOP", "NOP", "ORA", "ASL", "NOP", //0
	"BPL", "ORA", "NOP", "NOP", "NOP", "ORA", "ASL", "NOP", "CLC", "ORA", "NOP", "NOP", "NOP", "ORA", "ASL", "NOP", //1
	"JSR", "AND", "NOP", "NOP", "BIT", "AND", "ROL", "NOP", "PLP", "AND", "ROL", "NOP", "BIT", "AND", "ROL", "NOP", //2
	"BMI", "AND", "NOP", "NOP", "NOP", "AND", "ROL", "NOP", "SEC", "AND", "NOP", "NOP", "NOP", "AND", "ROL", "NOP", //3
	"RTI", "EOR", "NOP", "NOP", "NOP", "EOR", "LSR", "NOP", "PHA", "EOR", "LSR", "NOP", "JMP", "EOR", "LSR", "NOP", //4
	"BVC", "EOR", "NOP", "NOP", "NOP", "EOR", "LSR", "NOP", "CLI", "EOR", "NOP", "NOP", "NOP", "EOR", "LSR", "NOP", //5
	"RTS", "ADC", "NOP", "NOP", "NOP", "ADC", "ROR", "NOP", "PLA", "ADC", "ROR", "NOP", "JMP", "ADC", "ROR", "NOP", //6
	"BVS", "ADC", "NOP", "NOP", "NOP", "ADC", "ROR", "NOP", "SEI", "ADC", "NOP", "NOP", "NOP", "ADC", "ROR", "NOP", //7
	"NOP", "STA", "NOP", "NOP", "STY", "STA", "STX", "NOP", "DEY", "NOP", "txa", "NOP", "STY", "STA", "STX", "NOP", //8
	"BCC", "STA", "NOP", "NOP", "STY", "STA", "STX", "NOP", "TYA", "STA", "TXS", "NOP", "NOP", "STA", "NOP", "NOP", //9
	"LDY", "LDA", "LDX", "NOP", "LDY", "LDA", "LDX", "NOP", "TAY", "LDA", "TAX", "NOP", "LDY", "LDA", "LDX", "NOP", //A
	"BCS", "LDA", "NOP", "NOP", "LDY", "LDA", "LDX", "NOP", "CLV", "LDA", "TSX", "NOP", "LDY", "LDA", "LDX", "NOP", //B
	"CPY", "CMP", "NOP", "NOP", "CPY", "CMP", "DEC", "NOP", "INY", "CMP", "DEX", "NOP", "CPY", "CMP", "DEC", "NOP", //C
	"BNE", "CMP", "NOP", "NOP", "NOP", "CMP", "DEC", "NOP", "CLD", "CMP", "NOP", "NOP", "NOP", "CMP", "DEC", "NOP", //D
	"CPX", "SBC", "NOP", "NOP", "CPX", "SBC", "INC", "NOP", "INX", "SBC", "NOP", "NOP", "CPX", "SBC", "INC", "NOP", //E
	"BEQ", "SBC", "NOP", "NOP", "NOP", "SBC", "INC", "NOP", "SED", "SBC", "NOP", "NOP", "NOP", "SBC", "INC", "NOP"  //
	//0 ,  1  ,  2  ,  3  ,  4  ,  5  ,  6  ,  7  ,  8  ,  9  ,  A  ,  B  ,  C  ,  D  ,  E  ,  

};

int CPU::opcodeCycles[256] ={
	7, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 0, 4, 6, 0, //0
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, //1
	6, 6, 0, 0, 3, 3, 5, 0, 4, 2, 2, 0, 4, 4, 6, 0, //2
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,	//3
	6, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 3, 4, 6, 0, //4
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, //5
	6, 6, 0, 0, 0, 3, 5, 0, 4, 2, 2, 0, 5, 4, 6, 0, //6
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, //7
	2, 6, 0, 0, 3, 3, 3, 0, 2, 0, 2, 0, 4, 4, 4, 0, //8
	0, 6, 0, 0, 4, 4, 4, 0, 2, 5, 2, 0, 0, 5, 0, 0, //9
	2, 6, 2, 0, 3, 3, 3, 0, 2, 2, 2, 0, 4, 4, 4, 0, //A
	2, 5, 0, 0, 4, 4, 4, 0, 2, 4, 2, 0, 4, 4, 4, 0, //B
	2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 3, 6, 0, //C
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, //D
	2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 6, 0, //E
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, //F
//  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
};



int CPU::opcodePageCycles[256] ={
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0
	0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, //1
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //2
	0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, //3
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //4
	0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, //5
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //6
	0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, //7
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //8
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //9
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //A
	0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, //B
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //C
	0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, //D
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //E
	0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, //F
//  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
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
	 8, 10,  1, 10,  2,  2,  2,  2,  8,  1,  0,  1,  5,  5,  5,  5,  // 0
	 9, 11, 15, 11,  3,  3,  3,  3,  8,  7,  8,  7,  6,  6,  6,  6,  // 1
	 5, 10,  1, 10,  2,  2,  2,  2,  8,  1,  0,  1,  5,  5,  5,  5,  // 2
	 9, 11, 15, 11,  3,  3,  3,  3,  8,  7,  8,  7,  6,  6,  6,  6,  // 3
	 8, 10,  1, 10,  2,  2,  2,  2,  8,  1,  0,  1,  5,  5,  5,  5,  // 4
	 9, 11, 15, 11,  3,  3,  3,  3,  8,  7,  8,  7,  6,  6,  6,  6,  // 5
	 8, 10,  1, 10,  2,  2,  2,  2,  8,  1,  0,  1, 12,  5,  5,  5,  // 6
	 9, 11, 15, 11,  3,  3,  3,  3,  8,  7,  8,  7,  6,  6,  6,  6,  // 7
	 1, 10,  1, 10,  2,  2,  2,  2,  8,  1,  0,  1,  5,  5,  5,  5,  // 8
	 9, 11, 15, 11,  3,  3,  4,  4,  8,  7,  8,  7,  6,  6,  7,  7,  // 9
	 1, 10,  1, 10,  2,  2,  2,  2,  8,  1,  8,  1,  5,  5,  5,  5,  // A
	 9, 11, 15, 11,  3,  3,  4,  4,  8,  7,  8,  7,  6,  6,  7,  7,  // B
	 1, 10,  1, 10,  2,  2,  2,  2,  8,  1,  8,  1,  5,  5,  5,  5,  // C
	 9, 11, 15, 11,  3,  3,  3,  3,  8,  7,  8,  7,  6,  6,  6,  6,  // D
	 1, 10,  1, 10,  2,  2,  2,  2,  8,  1,  8,  1,  5,  5,  5,  5,  // E
	 9, 11, 15, 11,  3,  3,  3,  3,  8,  7,  8,  7,  6,  6,  6,  6,  // F
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
	return 0;
}

void CPU::initialize(Memory *memP){
	memory = memP;
	on_nmi = false;
	reset_vector = (memory->read(0xfffd)<<8) | memory->read(0xfffc);
	printf("RESET VECTOR = %x\n", reset_vector);
	printf("N=%d\n",N); //7
	printf("V=%d\n",V); //6
	printf("B=%d\n",B); //4
	printf("D=%d\n",D); //3
	printf("I=%d\n",I); //2
	printf("Z=%d\n",Z); //1
	printf("C=%d\n",C); //0*/

	test_stat = memory->read(0x6000);
	printf("INIT STATUS = %X\n",test_stat);

	reset();
}

void CPU::executeNMI(){
	// pushes the processor status register and return address on the stack
	uint8 pc_high = ((pc) & 0xFF00) >> 8;
	uint8 pc_low = (pc) & 0x00FF;
	memory->write(sp--, pc_high);
	memory->write(sp--, pc_low);
	push_status_to_Stack(true);
	//memory->write(sp--, P);
	printf("\nNMI CPU HERE \n\n");
	printf("pc low = %x\n",pc_low);
	printf("pc high=%x\n",pc_high);
	pc = (memory->read(0xfffb)<<8) | memory->read(0xfffa);
	//pc = (memory->read(0xffff)<<8) | memory->read(0xfffe);
	//fetchOpcode();
}

int CPU::branch(){

	//uint8 low = (pc & 0x00FF) + (operand & 0x00FF);
	//uint8 high = (pc>>8);//operand>>8);
	sint8 x = (sint8) operand;
	uint16 next = pc + x;
	int addCycle = 0;

	if (pageCrossed(pc,next)){
		addCycle = 2;
	}
	else{
		addCycle = 1;
	}
	//uint8 low = (pc & 0x00FF) + x;
	//printf("BRANCH: pc = %d + (%d) \n",pc,x);
	pc = next;
	return addCycle;

	//pc = high + low;
}

int CPU::emulateCycles(int cycles, std::string *str){
	//int cycle = 0;
	uint8 ppustatus = *memory->map[0x2002];
	uint8 ppuctrl = *memory->map[0x2000];
	//printf("PPUCTRL&0x80 = %x&0x80=%x\n",ppuctrl,ppuctrl&0x80);
	//printf("PPUSTATUS = %x\n",ppustatus);
	//printf("PPUCTRL = %x\n",ppuctrl);
	//printf("PPUSTATUS&0x80 = %x&0x80=%x\n",ppustatus,ppustatus&0x80);

	//printf("ppu_status = %x, ppu_ctrl= %x\n",ppustatus, ppuctrl);
	//printf("scanline = %d, dot = %d\n", )


	if(((ppustatus&0x80)==0x80) && ((ppuctrl&0x80)==0x80) && (!on_nmi)){
	//if(((ppustatus&0x80)==0x80) && (!on_nmi)){
	    //printf("PRE EXECUTE NMI\n");
	    on_nmi = true;
		executeNMI();
	}
	else if(((ppustatus&0x80)==0) && (on_nmi)){
		on_nmi = false;
	}

	//printf("\nPPUSTATUS emulate cycle = %x\n",ppustatus);
	int cclock = 0;
	char c1 = 's';
	int aux;
	int ncycles = 0;
	//unsigned int addr_brk = 0;
	uint16 curr_addr;



	while(ncycles < cycles){
		//2273
		//TODO: REMOVE THAT(should be handled by ppu), THIS IS JUST A HACK TO USE ROM TESTS FOR BRANCH TIMING
		//      CPU SHOULD NOT BE WRITING ON PPU STATUS REGISTERS
		if ((on_nmi) && ncycles > 2273) { // NUmber of cpu cycles for v-blank
			int ppu_Stat = memory->read(0x2002);
			ppu_Stat = ppu_Stat & 0x7F;
			memory->write(0x2002,ppu_Stat);
		}
		inst_counter++;
		curr_addr = pc;
		//printf("Total Instructions: %ld\n",inst_counter);
		//printf("1ppu cycle instructions: %ld\n",cinst);


		aux = fetchOpcode(str);
		//printf("CYCLE VAL = %d\n",aux);
		if(aux<0){
			return -1;
		}
		ncycles += aux;
		//printf("curr_addr = %x\n", curr_addr);
		/*#ifdef DEBUG
			if ((addr_brk==0)||(addr_brk==curr_addr)){
				printf("next: ");
				scanf("%x",&addr_brk);
				if (addr_brk==0xf){
					addr_brk = curr_addr;
				}
				printf("addr_brk = %x\n", addr_brk);
				//getchar();
			}
		#endif*/
		//c1 = getchar();
		//printf("ncycle %ld\n\n", ncycles);
		/*if (c1!='l'){
			c1 = getchar();
				//printf("\n");
		}*/
		++cclock;
	}
	//clock = 0;
	return ncycles;
}

void CPU::reset(){
	pc = reset_vector;
	//pc = (memory->read(0xfffb)<<8) | memory->read(0xfffa);
}

void CPU::setSignalFlags(uint8 value){
	if(value==0){
		status[Z]=1;
	}
	else{
		status[Z]=0;
	}
	status[N] = (value)>>7;
}
void CPU::push_status_to_Stack(bool isInt){
	uint8 P = 0; //cpuObj->memory->write(cpuObj->sp--, cpuObj->P);
	for(int i=0;i<8;i++){
        P |= (status[i]<<i);
    }
    P |= 0x30;

	/*uint8 bit4 = 1;
	if (isInt){
		bit4 = 0;
	}
	for(int i=0;i<8;i++){
		if(i==4){
			P |= (bit4<<i);
		}
		else{
			P |= (status[i]<<i);
		}
	}*/
	memory->write(sp, P);
	updateSP(0);
}

void CPU::pop_status_from_Stack(){
	updateSP(1);
	uint8 P = memory->read(sp);
	for(int i=0;i<8;i++){
		//if(i!=5){
		status[i] = (P>>i)&0x01;
		//}
	}
	//status[U] = 1;
	//status[B] = 1;
}

int CPU::fetchOpcode(std::string *str){

	int mode;
	//int addr = (memory->read(0xFFFF)<<8) | memory->read(0xFFFE);
	uint8 ppuctrl = *memory->map[0x2000];
	opcode = *memory->map[pc];
	mode = opcodeMode[opcode];
	uint16 aux = 0;
	int cycles = opcodeCycles[opcode];
	bool pcross = false;
	uint16 gpc = pc;
	char buff1[300];
	char buff2[30];
	//char pval[8];
	std::string pval;
	int j;
	uint8 stat1;
	uint8 result;
	char letter;
	char status_flags[9];

	//printf("opcode cycle = %d\n",cycles);
	//printf("OPCODE MODE IS %d\n",mode);
	//printf("$%X: %X\n",pc,opcode)



	/*if (str!=NULL){
		//message.write((const char*)&a, sizeof(a));
		//sstr.write("A"(const char*))
		j = 0;
		for (int i=7; i>=0; i--){
			if (status[i]==1){
				pval.push_back(stat_chart[i] - 32);

				//*str << std::uppercase << this->status[i];
			}
			else{
				//*str << this->status[i];
				//pval[j] = stat_chart[i];
				pval.push_back(stat_chart[i]);
			}
			j++;
		}
		//pval[9] = '\0';
		sprintf(buff1,"A:%02X X:%02X Y:%02X S:%02X P:%s $%04X:%02X ",A,X,Y,(sp-0x100),pval.c_str(),pc,opcode);
		str->append(buff1);
	}*/

		/**str << "A:" << std::hex << (const char*)A;//<< std::hex << std::uppercase << std::setfill('0') << std::setw(2) << A;
		*str << " X:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << X;
		*str << " Y:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << Y;
		*str << " S:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << sp;
		*str << " P:";&/
		for (int i=0; i<8; i++){
			if (this->status[i]){

				//*str << std::uppercase << this->status[i];
			}
			else{
				//*str << this->status[i];
			}
		}
		*str << "      $" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << pc << ":" << opcode << "\n";
	}*/

	//printf("$%X:%x \n", pc,opcode);

	#ifdef DEBUG
		debug_str.clear();
		/*int idx=0;
		printf("A:%X X:%X Y:%X S:%X ",A,X,Y,sp);
		printf("P:");
		for(idx=7;idx>=0;idx--){
			printf("%c", ascii_flag_map[idx] - status[idx]*32);
		}
		printf("				");

		printf("$%x:%x ",pc,opcode);*/
		for (int i=7; i>=0; i--){
			status_flags[7-i] = ascii_flag_map[i] - status[i]*32;
		}
		status_flags[8] = '\0';
		sprintf(buff1,"A:%02X X:%02X Y:%02X S:%02X P:%s $%04X:%02X ",A,X,Y,sp,status_flags,pc,opcode);
		debug_str.append(buff1);
		//printf("1FF = %02X\n",*memory->map[0x1ff]);
		//printf("%s",debug_str.c_str());
		/*printf("OPCODE = %X\n", opcode);
		printf("pc = %x \n", pc);
		printf("sp = %x \n", sp);
		printf("P = %x \n", P);
		printf("A = %x \n", A);
		printf("X = %x \n",X);
		printf("Y = %x \n",Y);
		printf("banks number = %d \n", memory->cartridge->n_banks);
		printf("chr banks number = %d \n", memory->cartridge->chr_banks);
		printf("PPUCONTROL = %x\n", ppuctrl);
		printf("PPUstatusL = %x\n", *memory->map[0x2002]);
		printf("N=%d\n",status[N]); //7
		printf("V=%d\n",status[V]); //6
		printf("U=%d\n",status[U]); //4
		printf("B=%d\n",status[B]); //4
		printf("D=%d\n",status[D]); //3
		printf("I=%d\n",status[I]); //2
		printf("Z=%d\n",status[Z]); //1
		printf("C=%d\n",status[C]); //0*/
	#endif
	

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

	//operand = -5;
	switch(mode){
		case accumulator:
			// Accumulator
			//if (DEBUG)
				//cout << "Mode: accumulator\n";
			operand = -2;
			jumpTable[opcode](this);
			#ifdef DEBUG
			sprintf(buff1,"%s", opcodeNameTable[opcode].c_str());
			#endif
			pc+=1;
			break;
		case immediate:
			// Immediate
			operand = *memory->map[pc+1];
			/*#ifdef DEBUG
				printf("%x ",operand);
				/*cout << "Mode: immediate\n";
				printf("operand = %x\n",operand);
			#endif*/
			result = memory->read(operand);
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);

			#ifdef DEBUG
			sprintf(buff1,"%02X %s #$%02X", operand, opcodeNameTable[opcode].c_str(), operand);
			#endif
			pc+=2;
			break;
		case zpage:
			// Zero Page
			//cout << "Mode: zpage\n";
			operand = *memory->map[pc+1];
			result = memory->read(operand);
		
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);

			#ifdef DEBUG
			sprintf(buff1,"%02X %s $%04X = #$%02X", operand, opcodeNameTable[opcode].c_str(), operand, result);
			#endif
			/*if (DEBUG) {
				printf("%x ",operand);
				/*cout << "Mode: zpage\n";
				printf("MEM[%x] = %x\n", operand, result);
				printf("MEM[%x] = %x\n", operand, memory->read(operand));*
			}*/
			pc+=2;
			break;
		case zpageX:
			// Indexed Zero Page with X
			//cout << "Mode: zpageX\n";
			operand = (*memory->map[pc+1] + X)&0x00ff;
			result = memory->read(operand);
			//printf("operand = %x\n", operand);
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);

			#ifdef DEBUG
			sprintf(buff1,"%02X %s $%04X,X @ $%04X = #$%02X", *memory->map[pc+1], 
				    opcodeNameTable[opcode].c_str(), *memory->map[pc+1], operand, result);
			#endif
			/*if (DEBUG){
				cout << "Mode: zpageX\n";
				printf("operand = %x\n", operand);
				printf("MEM[%x] = %x\n", operand, result);
				printf("MEM[%x] = %x\n", operand, memory->read(operand));
			}*/
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			pc+=2;
			break;
		case zpageY:
			// Indexed Zero Page with Y
			//cout << "Mode: zpageY\n";
			operand = (*memory->map[pc+1] + Y)&0x00ff;
			result = memory->read(operand);
			//printf("operand = %x\n", operand);
			/*if (DEBUG){
				cout << "Mode: zpageY\n";
				printf("operand = %x\n", operand);
			}*/
			jumpTable[opcode](this);
			#ifdef DEBUG
			sprintf(buff1,"%02X %s $%04X,Y @ $%04X = #$%02X", *memory->map[pc+1], 
				    opcodeNameTable[opcode].c_str(), *memory->map[pc+1], operand, result);
			#endif
			pc+=2;
			break;
		case absolute:
			// Absolute
		{
			//cout << "Mode: absolute\n";
			//printf("%04X memory->map[pc+1] = %X , memory->map[pc+2] = %X\n", operand, *memory->map[pc+1], *memory->map[pc+2]);
			uint8 pc1 = *memory->map[pc+1];
			uint8 pc2 =  *memory->map[pc+2];
			uint16 less_sig = *memory->map[pc+1];
			uint16 more_sig = *memory->map[pc+2] << 8;
			operand = more_sig | less_sig;
			result = memory->read(operand);
			//printf("MEM[%x] = %x\n", operand, *memory->map[operand]);
			jumpTable[opcode](this);
			/*if (DEBUG){
				cout << "Mode: absolute\n";
				printf("MEM[%x] = %x\n", operand, result);
				printf("MEM[%x] = %x\n", operand, memory->read(operand));
			}*/
			#ifdef DEBUG
			sprintf(buff1,"%02X %02X %s $%04X = #$%02X absolute", pc1,pc2,
				    opcodeNameTable[opcode].c_str(), operand, result);
			#endif
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			pc+=3;
		}
			break;
		case absoluteX:
			// Indexed Absolute X:
		{
			//cout << "Mode: absoluteX\n";
			uint16 less_sig = *memory->map[pc+1];
			uint16 more_sig = *memory->map[pc+2]<<8;
			aux = more_sig | less_sig;
			operand = aux + X;
			result = memory->read(operand);
			//printf("pc %x: %x\n",(pc+1),*memory->map[pc+1]);
			//printf("pc %x: %x\n",(pc+2),*memory->map[pc+2]);
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			/*if (DEBUG){
				cout << "Mode: absolute\n";
				printf("MEM[%x] = %x\n", operand, result);
				printf("MEM[%x] = %x\n", operand, memory->read(operand));
			}*/
			#ifdef DEBUG
			sprintf(buff1,"%02X %02X %s $%04X,X @ $%04X = #$%02X absolute X", less_sig, *memory->map[pc+2], 
				    opcodeNameTable[opcode].c_str(), aux, operand, result);
			#endif
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			pcross = pageCrossed(aux, operand);
			pc+=3;
		}
			break;
		case absoluteY:
			// Indexed Absolute Y
		{
			//cout << "Mode: absoluteY\n";
			uint16 less_sig = *memory->map[pc+1];
			uint16 more_sig = *memory->map[pc+2]<<8;
			aux = more_sig | less_sig;
			operand =  aux + Y;
			result = memory->read(operand);
            //printf("pc %x: %x\n",(pc+1),*memory->map[pc+1]);
			//printf("pc %x: %x\n",(pc+2),*memory->map[pc+2]);
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			/*if (DEBUG){
				cout << "Mode: absolute\n";
				printf("MEM[%x] = %x\n", operand, result);
				printf("MEM[%x] = %x\n", operand, memory->read(operand));
			}*/
			#ifdef DEBUG
			sprintf(buff1,"%02X %02X %s $%04X,Y @ $%04X = #$%02X", less_sig, *memory->map[pc+2], 
				    opcodeNameTable[opcode].c_str(), aux, operand, result);
			#endif
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			pcross = pageCrossed(aux, operand);
			pc+=3;
		}
			break;
		case implied:
			// Implied
			/*if (DEBUG)
				cout << "Mode: Implied\n";*/
			//operand = -1;
			jumpTable[opcode](this);
			#ifdef DEBUG
			sprintf(buff1,"%s", opcodeNameTable[opcode].c_str());
			#endif
			pc+=1;
			break;
		case relative:
			//cout << "Mode: relative\n";
			operand = *memory->map[pc+1];
			result = memory->read(operand);
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			cycles += jumpTable[opcode](this);

			#ifdef DEBUG
			sprintf(buff1,"%02X %s $%04X", operand, opcodeNameTable[opcode].c_str(), ((uint8) pc + operand));
			#endif
			/*if (DEBUG){
				cout << "Mode: relative\n";
				printf("MEM[%x] = %x\n", operand, result);
				printf("MEM[%x] = %x\n", operand, memory->read(operand));
			}*/
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			pc+=2;
			break;
		case indirectX:
			// Pre-indexed indirect, X
		{
			//cout << "Mode: indirectX\n";
			aux = *memory->map[pc+1];
			uint16 less_sig = *memory->map[(*memory->map[pc+1]+X)&0x00ff];
			uint16 more_sig = *memory->map[(*memory->map[pc+1]+X+1)&0x00ff] << 8;
			operand = more_sig | less_sig;
			result = memory->read(operand);
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			/*if (DEBUG){
				cout << "Mode: indirectX\n";
				printf("MEM[%x] = %x\n", operand, result);
				printf("MEM[%x] = %x\n", operand, memory->read(operand));
			}*/
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			#ifdef DEBUG
			sprintf(buff1,"%02X %s ($%02X,X) @$%04X = #$%02X indirect X", aux, 
				    opcodeNameTable[opcode].c_str(),aux, operand, result);
			//sprintf(buff1,"%02X %02X %s $%04X = #$%02X indirect X", less_sig, *memory->map[*memory->map[pc+1]+X+1], 
			//	    opcodeNameTable[opcode].c_str(), operand, result);
			#endif
			pc+=2;
		}
			break;
		case indirectY:
			// Post-indexed indirect, Y
		{
			//cout << "Mode: indirectY\n";
			aux = *memory->map[pc+1];
			uint16 less_sig = *memory->map[*memory->map[pc+1]&0x00ff];
			uint16 more_sig = *memory->map[(*memory->map[pc+1]+1)&0x00ff] << 8;
			//aux = more_sig | less_sig;
			operand = (more_sig | less_sig) + Y;
			result = memory->read(operand);
			//printf("xx: MEM[%x] = %x\n", *memory->map[pc+1], less_sig);
			//printf("yy: MEM[%x] = %x\n", *memory->map[pc+1]+1, more_sig);
			//printf("aux= %x\n", aux);
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);

			#ifdef DEBUG
			sprintf(buff1,"%02X %s ($%02X,X) @$%04X = #$%02X indirect Y", aux, 
				    opcodeNameTable[opcode].c_str(),aux, operand, result);
			//sprintf(buff1,"%02X %02X %s $%04X = #$%02X indirect Y", less_sig, *memory->map[*memory->map[pc+1]+1], 
			//	    opcodeNameTable[opcode].c_str(), operand, result);
			#endif
			/*if (DEBUG){
				cout << "Mode: indirectX\n";
				printf("MEM[%x] = %x\n", operand, result);
				printf("MEM[%x] = %x\n", operand, memory->read(operand));
			}*/

			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			pcross = pageCrossed(aux, operand);
			pc+=2;
		}
			break;
		case absoluteIndirect:
			// Absolute Indirect
		{

			uint16 pointer = (*memory->map[pc+2] << 8) | *memory->map[pc+1];

			/*This is to emulate a bug in which: "6502 has does not correctly fetch the target address if the indirect vector 
			falls on a page boundary (e.g. $xxFF where xx is any value from $00 to $FF). In this case fetches the LSB from $xxFF
			 as expected but takes the MSB from $xx00"*/
			aux = (pointer+1)&0x00ff | (pointer)&0xff00;

			operand = (*memory->map[aux] << 8) | *memory->map[pointer];
			//result = memory->read(operand);
			//printf("operand = %x\n", operand);
		    //printf("MEM[%x] = %x\n", operand, memory->read(operand));
			jumpTable[opcode](this);
			/*if (DEBUG){
				cout << "Mode: indirectX\n";
				printf("MEM[%x] = %x\n", operand, result);
				printf("MEM[%x] = %x\n", operand, memory->read(operand));
			}	*/
			//printf("MEM[%x] = %x\n", operand, memory->read(operand));
			#ifdef DEBUG
			sprintf(buff1,"%02X %02X %s ($%04X) = $%04X Absolute indirect", *memory->map[pc+1], 
				*memory->map[pc+2], opcodeNameTable[opcode].c_str(), pointer, operand);
			#endif
			pc+=3;
		}
			break;
		default:
			nmode = 0;
			//cycles = -1;
			pc++;
			//cout << "NO MODE!  ";
			//cout << "nmode = " << nmode << "\n";
			//return -1;
			//pc+=1;

	}
	#ifdef DEBUG
	debug_str.append(buff1);
	printf("%s\n",debug_str.c_str());
	#endif

	if (str!=NULL){
		if (operand >=0){
			sprintf(buff2,"MA:%04X" " MD:%02X STAT:%02X\n",operand, memory->read(operand), memory->read(0x6000));
			str->append(buff2);
		}
		else{
			str->append("\n");
		}
	}

	if (pcross){
		cycles += opcodePageCycles[opcode];
	}


	stat1 = memory->read(0x6000);


	//printf("TEST STATUS = %x\n\n", stat1);
	if(test_stat != stat1){
		printf("\nSTATUS = %X\n",memory->read(0x6000));
		if (test_stat==0x80){
			aux = 0;
			//if (stat1!=0){
				result = memory->read(0x6004);
				letter = test_translation[result];
				printf("\nERROR MSG: ");
				while (result!=0){
					//printf("%x: %x\n", 0x6004+aux, result);
					printf("%c",letter);
					aux+=1;
					result = memory->read(0x6004 + aux);
					letter = test_translation[result];
				}
				printf("\n");
			//}
            return -1;
		}
	}
	test_stat = stat1;
	//cout << "nmode = " << nmode << "\n";
	//printf("operand = %x\n",operand);

	//printf("STACK:\n");
	/*for (int i=(sp+1); i<=0x1ff; i++){
		printf("%x = %x\n", i, memory->read(i));
	}
	printf("\n");
	//printf("addr: %x, opcode: %x , opcodecycle = %d +CYCLES = %d \n\n",gpc,opcode,opcodeCycles[opcode],cycles);
	printf("TEST STATUS = %x\n\n", memory->read(0x6000));*/

	//str  << " X:" << this->X << " Y:" << this.Y << "S:" << FB P:nvubdIZc     $E771:A9 00     LDA #$00

	//cout <<"\n";
	return cycles;
}
