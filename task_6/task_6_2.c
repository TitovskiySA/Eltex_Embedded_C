#include <stdio.h>
#include <stdlib.h>

#define MAXSTR = 100

//struct somestr{
//	int num;
//	*char str;
//};


char *enter_dynamic_string(int *len){
	printf("\nstart of enter_dyn\n");
	*len = 0; // empty string
	int len_mem = 1; //size of string with symbol "\0"
	char *str = (char*)malloc(sizeof(char)); // creating pointer on our str
	char symb;
	printf("\nscanf symb\n");
	fgets(&symb, sizeof(char), stdin); // enter some symbol
	printf("\nend of getchar\n");

	//end of string will be "\n"
	while (symb != '\n'){
		printf("\n--iter--\n");
		str[(*len)++] = symb; //adding to string entered symbol, len is growing
		len_mem = len_mem++; //so increasing size with symbol "\0"
		str = (char*)realloc(str, len_mem * sizeof(char)); //increasing memory for string
		fgets(&symb, sizeof(char), stdin); //ask for new symbol
	} 
	
	len_mem++;
	str = (char*)realloc(str, len_mem * (sizeof(char)));
	str[*len] = '\0'; // add end of string
	return str; // return pointer on our string

}

void task(){
	int MSize = 0;
	printf("Enter size of massive\n");
	scanf("%d", &MSize);
	
	for (int i = 0; i < MSize; i++){
		printf("\nEnter element #%d\n", i);
		printf("Enter number of struct: ");
		int num = 0;
		scanf("%d", &num);
		printf("\nEnter string of struct: ");
		int len;
		char *str = enter_dynamic_string(&len);
		printf("you entered: %d, %s", num, str);
		free(str); 

	}


	/*struct somestr *massive = (struct somestr*)calloc(MSize, sizeof(struct somestr));
	if (massive == NULL){
		printf("Error allocate memory\n");
		return;
	}
	struct somestr *start_massive = massive;
	for (int i = 0; i < MSize; i++){
		int i_char = i % 100;
		if (i % 100 > 73) i_char = i % 100 - 73; //this is for beautiful char
		struct somestr item = {.num = i, .symb = i_char + 58};
		*massive = item; 
		printf("%d, %c \n", massive -> num, massive -> symb);
		massive++;
	}*/

	printf("\n");
//	free(start_massive);
	return;
}

int main(){
	task();
	return 0;
}
