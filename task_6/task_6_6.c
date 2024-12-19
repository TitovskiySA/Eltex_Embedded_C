#include <stdio.h>
#include <stdlib.h>

void enter_dynamic_string(char ** str_p){ //foo saves in this string
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
	printf("Enter size of static massive\n");
	int size = 0;

	for (;;){
		scanf("%d", &size);
		if (size < 1){
			printf("You entered size < 1, please enter new size value");
		}
		else {
			printf("You entered massive size =  %d", size);
			break;
		}
	}
	
	char * strings[size]; // creating static massive

	for (int i = 0; i < size; i ++){
		printf("\nprint string #%d\n", i);
		enter_dynamic_string(&(strings[i])); //store to pointer of element entered string
		//printf("You entered = %s", strings[i]);
	}
	
	for (int i = 0; i < size; i ++){
		printf("strings #%d = %s", i, strings[i]);
		free(strings[i]); //free memory of dynamic string
		strings[i] = NULL;
	}
	return;	
}
	
int main(){
	task();
	return 0;
}
