#include <stdio.h>
#include <stdlib.h>

void enter_dynamic_string(char ** str_p){ //foo saves in this pointer address where entered dynamic string store
        char *str = (char*)malloc(sizeof(char) * 2); // creating pointer on our str
        int symb; // getchar returns int
        for (;;){
                symb = getchar();
                if (symb != '\n') break; // if read not empty symbol, it is first entered by user char
                //else printf("\nignoring some empty symb");
        }
        str[0] = symb; // this symbol will be first in our string
        str[1] = '\0'; // at the end must be this symbol - EOS
        int i = 1; // for next cycle
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
	int num = 2;
	printf("Firstfull let's make massive of %d elements (string)\n", num);
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
		printf("\nmassive #%d: %s", i, **massive);
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
                printf("\nmassive #%d: %s", i, **massive);
                free(**massive); //free memory of string
		**massive = NULL;
		free(*massive); //free memory for pointer on string
		*massive = NULL;
                massive++;
        }

	free(massive_begin); //free memory of massive
	massive_begin = NULL;
        return;	
	}

int main(){
	task();
	return 0;
}
