#include <stdio.h>

void number_of_1_bite(unsigned int);

int main(){
	unsigned int num;
	printf("Enter sum num\n");
	scanf("%u", &num);
	printf("you entered num = %u\n", num);
	number_of_1_bite(num);
	return 0;
}

void number_of_1_bite(unsigned int n){
	short quantity = 0;
	for (size_t i = 0; i < sizeof(n) * 8; i++){
		int res = n & (1 << i);
		if (res > 0){
			quantity++;
			printf("1");
		}
		else printf("0");
	}
	
	if (quantity % 2 == 1){
		printf("\nodd number of 1 (%d)\n", quantity);
		return;}
	printf("\neven number of 1 (%d)\n", quantity);
	return;

}
