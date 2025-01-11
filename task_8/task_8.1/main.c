#include <stdio.h>
//#include <ctype.h>
#include "libs/calc.h"
#include "libs/io.h"
#include "libs/helper.h"

int main(){
	say_hello();
	int exitcode = 1;

	while (exitcode != 0){
	        /*read numbers from colsole*/
		printf("Enter to numbers for calculate\n");
		int num1 = input_num("a");
		int num2 = input_num("b");

		/*ask user what action user wants*/
		int chosen = 0;
		printf("Choose action:\n1. add\n2. subtraction\n3. multiply\n4. divide\n0. Exit\n");
		scanf("%*1[\n]");
		scanf("%d", &chosen);
        	switch (chosen)
        	{
        		case 1:
              		printf("%d + %d = %d\n", num1, num2, add(num1, num2));
              		break;
        		case 2:
              		printf("%d - %d = %d\n", num1, num2, sub(num1, num2));
              		break;
        		case 3:
              		printf("%d * %d = %d\n", num1, num2, mul(num1, num2));
              		break;
        		case 4:
              		printf("%d / %d = %f\n", num1, num2, div(num1, num2));
              		break;
			case 0:
	      		exitcode = 0;
	      		break;
        		default:
              		printf("Entered something another, not 1-4\n");
              		break;
		}
	}
	printf("End of script\n");
	return 0;
}
