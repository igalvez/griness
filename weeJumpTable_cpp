#include <stdio.h>



int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

int (*weeJumpTable[4])(int, int) = {add, subtract, multiply, divide};


int main(void){
	int op=0;
	int a, b;
	while(op<1 || op>4){
		printf("Choose operation: \n");
		printf("1 - Addition\n");
		printf("2 - Subtraction\n");
		printf("3 - Multiplication\n");
		printf("4 - Division\n");

		scanf("%d", &op);
		//op--;
	}
	--op;
	printf("First number for the operation: ");
	scanf("%d", &a);
	printf("Second number for the operation: ");
	scanf("%d", &b);
	printf("op is %d\n", op);
	weeJumpTable[op--](a,b);
	printf("op is now %d", op);

}



int add(int a, int b){
	printf ("a + b = %d\n", a+b);
	return a + b;
}

int subtract(int a, int b){
	printf ("a - b = %d\n", a-b);
	return a - b;
}

int multiply(int a, int b){
	printf ("a * b = %d\n", a*b);
	return a*b;
}

int divide(int a, int b){
	printf ("a / b = %d\n", a/b);
	return a/b;
}