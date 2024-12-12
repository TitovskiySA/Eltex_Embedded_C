#include <stdio.h>
#include <stdlib.h>

struct somestr{
	int num;
	char symb;
};

void task(){
	int MSize = 0;
	printf("Enter size of massive\n");
	scanf("%d", &MSize);
	struct somestr *massive = (struct somestr*)calloc(MSize, sizeof(struct somestr));
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
	}

	printf("\n");
	free(start_massive);
	return;
}

int main(){
	task();
	return 0;
}
