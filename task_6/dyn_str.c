#include <stdio.h>
#include <stdlib.h>

char *enter_dynamic_string(int *len){
	//printf("\nstart of enter_dyn\n");
	char *str = (char*)malloc(sizeof(char)); // creating pointer on our str
	int symb;
	str[0] = '\0'; // str must always have this at the end
	int i = 0;
	for (;;){
		if (symb == '\n') break; // end of entered string
		str = realloc(str, (i + 2) * sizeof(char)); // memory for our string + ending symbol
		symb = getchar(); // enter one char
		str[i] = (char)symb; // writing our symbol in string
		str[i+1] = '\0'; // adding finishing symbol
		i++; // next cycle
	}
	*len = i; // store of lengh of string	
	return str;
}

void task(){
	int str_len = 0;
	printf("\nEnter some string\n");
	char *str = enter_dynamic_string(&str_len);
	printf("You entered string (lengh = %d)\n%s", str_len, str);
	free(str);
        return;	
	}

int main(){
	task();
	return 0;
}
