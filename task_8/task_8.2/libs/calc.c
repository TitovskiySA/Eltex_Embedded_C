#include "calc.h"
#include <stdio.h>

int add(int a, int b){
	printf("add function from calc.c started\n");
	return a + b;
}

int sub(int a, int b){
	printf("sub function from calc.c started\n");
	return a + b;
}

int mul(int a, int b){
	printf("mul function from calc.c started\n");
	return a * b;
}

double div(int a, int b){
	printf("div function from calc.c started\n");
	return (double)a / (double)b;
}
