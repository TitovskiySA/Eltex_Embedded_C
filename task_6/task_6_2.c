#include <stdio.h>
#include <stdlib.h>

struct some{
	int num;
	char** str;
};

/*char **/void enter_dynamic_string(char ** str_p/*int *len*/){
        //printf("\nstart of enter_dyn\n");
        char *str = (char*)malloc(sizeof(char)); // creating pointer on our str
	//printf("created pointer on %p\n", str);
        int symb;
	//printf("ignoring symb %c = ", (char)symb);
        str[0] = '\0'; // str must always have this at the end
        int i = 0;
	getchar(); // ignoring first symbol after scanf
        for (;;){
                if (symb == '\n') break; // end of entered string
                str = realloc(str, (i + 2) * sizeof(char)); // memory for our string + ending symbol
                symb = getchar(); // enter one char
                str[i] = (char)symb; // writing our symbol in string
                str[i+1] = '\0'; // adding finishing symbol
                i++; // next cycle
        }
//        *len = i; // store of lengh of string   
	*str_p = str;
	printf("\n*str_p in foo = %p\n", str);
	printf("string in p = %s\n", str);
	return;
//        return str;
}

	

void task(){
	int mass_len = 0;
	int size = sizeof(struct some);
	struct some *massive = (struct some *)malloc(size);
        struct some *massive_begin = massive;	
	printf("pointer begin = %p\n", massive_begin);
	int str_len = 0;
	int num = 0;
	char* some_p;
	

	printf("Enter number of massive elements\n");
	scanf("%d", &mass_len);
	for (int i = 0; i < mass_len; i++){
		printf("\nEnter .num for #%d item: ", i);
		scanf("%d", &num);
		//str_len = 0;
		printf("\nEnter .str for #%d item: ", i);
		char** str_p = (char **)malloc(sizeof(char));
		printf("\nstr_p = %p\n", str_p);
		enter_dynamic_string(str_p); 
//		struct some item = {num, str_p};
		printf("string at %p", str_p);
//		massive = massive_begin + i;
//		*massive = item;
//		printf("\n struct #%d: %d %p", i, massive->num, massive->str);
//		size = size + sizeof(struct some);
		//printf("\n size = %d", size);
//		massive_begin = realloc(massive_begin, size);
//		printf("\nnow massive %p", massive);
//		printf("\nnow mass_begin %p", massive_begin);
//		massive = massive_begin + 1 + i;
	
	}
//	massive = massive_begin;
	//printf("all size = %d", size);
	//printf("\nnow massive = %p", massive);
	//printf("\nnow massive_begin = %p", massive_begin);
//	for (int i = 0; i < mass_len; i++){
//		printf("\nstruct #%d: %d, %p", i, massive->num, massive->str);
//		massive++;
//	}
	printf("\n");

	free(massive_begin);
        return;	
	}

int main(){
	task();
	return 0;
}
