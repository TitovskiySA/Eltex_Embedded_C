#include <stdio.h>

int main(void){
	float x = 5.0;
	printf("x = %f\n", x);
	float y = 6.0;
	printf("y = %f\n", y);
	float *xp = &y; // changed &y from &x
	float *yp = &y; 
	printf("result: %f\n", *xp + *yp);
}

