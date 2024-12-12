#include <stdio.h>
#include <stdlib.h>

#define MAXSTR = 100

struct somestruct{
	int num;
	*char str;
};

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
	int mass_len = 0;
	int size = sizeof(struct somestruct);
	struct somestruct *massive = (struct somestruct *)malloc(size);
        struct somestruct *massive_begin = massive;	
	int num = 0;
	int str_len = 0;
	//char *str;
	printf("Enter number of massive elements\n");
	scanf("%d", &mass_len);
	for (int i = 0; i < mass_len; i++){
		printf("\nEnter .num for #%d item: ", i);
		scanf("%d", &num);
		str_len = 0;
		printf("\nEnter .str for #%d item: ", i);
		char *str = enter_dynamic_string(&str_len);
		//massive = struct somestr elem;
		massive -> num = num;
		massive -> str = str;
		printf("\n struct #%d: %d %s", i, num, str);
		size = size + sizeof(struct somestruct);
		massive = realloc(massive_begin, size);
		printf("\n Go to next element\n");
		massive++;

	}

	free(massive_begin);
        return;	
	}

int main(){
	task();
	return 0;
}
