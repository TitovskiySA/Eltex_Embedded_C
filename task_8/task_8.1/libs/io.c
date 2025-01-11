#include "io.h"
#include <stdio.h>

int input_num(char* label){
	int res = 0;
	printf("Enter number %s:\n", label);
	scanf("%*1[\n]");
	scanf("%d", &res);
	printf("\nYou entered: %d\n", res);
	return res;
}

void sh_result(int res){
	printf("Result of your operation %d", res);
	return;
}
