#include <stdio.h>
#include <stdlib.h>

struct some{
	int num; //here int
	char** str; //here pointer on where dynamic string stores
};

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
	int mass_len = 0;
	int size = sizeof(struct some);
	struct some *massive = (struct some *)malloc(size);
	if (massive == NULL){
		printf("Error allocating memory for massive\n");
		return;
	}
        struct some *massive_begin = massive;//stores begin of massive	
	int num = 0;//for entering first element of struct
	printf("Enter number of massive elements\n");
	scanf("%d", &mass_len);
	for (int i = 0; i < mass_len; i++){
		printf("\nEnter .num for #%d item: ", i);
		scanf("%d", &num);
		printf("\nEnter .str for #%d item: ", i);
		char** str_p = (char **)malloc(sizeof(char));//allocate memory for second element of struct
		if (str_p == NULL){
			printf("Error allocating memory for str_p\n");
			return;
		}
		enter_dynamic_string(str_p);//enter our string and save it addr to pointer str_p 
		struct some item = {num, str_p};//creating element struct
		massive = massive_begin + i;//iteration to next element of massive
		*massive = item;//saving struct element to massive
		size = size + sizeof(struct some);//increasing size of massive for next cycle
		massive_begin = realloc(massive_begin, size);//reallocating memory for new size massive 
		if (massive_begin == NULL){
			printf("Error reallocating memory for massive_begin\n");
			return;
		}
	
	}
	massive = massive_begin;
	for (int i = 0; i < mass_len; i++){
		printf("\nstruct #%d: %d, %s", i, massive->num, *(massive->str));
		free(massive->str); //free memory of string
		massive++;
	}
	printf("\n");

	free(massive_begin); //free memory of massive
        return;	
	}

int main(){
	task();
	return 0;
}
