#include "helper.h"
#include <stdio.h>
#include <stdlib.h>

//not necessary function
static void sh_info(){
	printf("----------\n");
	printf("Here's some info about file\n");
	printf("----------\n");
	return;
}

static void enter_dynamic_string(char ** str_p){ //foo saves in this string
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
void say_hello(){
        char* name;
        printf("What is your name?\n");
        enter_dynamic_string(&name);
        printf("\nHello, %s!!!\n", name);
        free(name);
        return;
}

