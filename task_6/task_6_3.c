#include <stdio.h>
#include <stdlib.h>

void enter_dynamic_string(char ** str_p){ //foo saves in this pointer address where entered dynamic string store
        char *str = (char*)malloc(sizeof(char)); // creating pointer on our str
        int symb; // getchar returns int
        str[0] = '\0'; // it must always have this at the end of string
        int i = 0; //for cycle
	while (getchar() == '\n'){//this cycle delete all \n symbols
		//printf("\ndeleting some empty symbol");
	}
        for (;;){
                if (symb == '\n') break; // end of entered string
                str = realloc(str, (i + 2) * sizeof(char)); // memory for our string + ending symbol
                symb = getchar(); // enter one char
                str[i] = (char)symb; // writing our symbol in string
                str[i+1] = '\0'; // adding finishing symbol
                i++; // next cycle
        }   
	*str_p = str; // save to entered pointer address of result string
	return;
}

void task(){
	printf("Firstfull let's make massive of 3 strings\n");
	int num = 3;
	char ***massive = (char ***)malloc(sizeof(char**) * num);
	if (massive == NULL){
		printf("Error allocating memory for massive\n");
		return;
	}
        char ***massive_begin = massive;//stores begin of massive	
	for (int i = 0; i < num; i++){
		printf("\nEnter str for #%d item: ", i);
		char** str_p = (char **)malloc(sizeof(char));//allocate memory for string
		if (str_p == NULL){
			printf("Error allocating memory for str_p\n");
			return;
		}
		enter_dynamic_string(str_p);//enter our string and save it addr to pointer str_p 
		massive = massive_begin + i;//iteration to current element of massiveI
		*massive = str_p;// saving to massive pointer of str
	}
	massive = massive_begin;
	for (int i = 0; i < num; i++){
		printf("\nmassive #%d: %p, %s", i, *massive, **massive);
		//free(massive); //free memory of string
		massive++;
	}
	printf("\n");
	int chosen = 0;
	for(;;){
		printf("Enter 1 if you want to add one item to massive, enter 0 for stop adding");
		scanf("%d", &chosen);
		if (chosen == 0){
			printf("\nEnd of adding items");
			break;
		}
		else if (chosen == 1){
			printf("\nEnter str for #%d item: ", num);
                	char** str_p = (char **)malloc(sizeof(char));//allocate memory for string
                	if (str_p == NULL){
                        	printf("Error allocating memory for str_p\n");
                        	return;
                	}
                	enter_dynamic_string(str_p);//enter our string and save it addr to pointer str_p 
			num++;
			massive_begin = (char ***)realloc(massive_begin, sizeof(char**) * num);
                	massive = massive_begin + num - 1;//iteration to current element of massiveI
                	*massive = str_p;// saving to massive pointer of str
		}
		else continue;

	}
	massive = massive_begin;
        for (int i = 0; i < num; i++){
                printf("\nmassive #%d: %p, %s", i, *massive, **massive);
                //free(massive); //free memory of string
                massive++;
        }


	free(massive_begin); //free memory of massive
        return;	
	}

int main(){
	task();
	return 0;
}
