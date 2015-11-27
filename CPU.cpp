#include "CPU.h"

static int CPU::(*jumpTable[256])() ={
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


CPU::CPU(Memory *memP){
	memoryPointer = memP;
}

void CPU::emulateCycle(){

}