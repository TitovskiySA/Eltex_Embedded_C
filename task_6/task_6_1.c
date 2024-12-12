#include <stdio.h>
#include <stdlib.h>

void task(){
	int MSize = 0;
	printf("Enter size of massive\n");
	scanf("%d", &MSize);
	int *massive = malloc(MSize * sizeof(int));
	if (massive == NULL){
		printf("Error allocate memory\n");
		return;
	}
	int *start_massive = massive;
	for (int i = 0; i < MSize; i++){
		*massive = i + 10;
		printf("%d ", *massive);
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
