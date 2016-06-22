#include <stdio.h>
#include <string>


void printVar(int &var){
	int *nvar;
	nvar = &var ;
	printf("Var is %d\n",var);
	printf("Addr is %p\n", &var);
	*nvar = var + 2;
}

int main(int argc,char *argv[]){
	int v;
	v = 2;
	printVar(v);
	printf("NOW var is %d\n",v);
}




