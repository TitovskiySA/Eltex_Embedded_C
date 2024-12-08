#include <stdio.h>

void number_of_1_bite(unsigned int);

int main(){
	unsigned int num;
	printf("Enter sum num\n");
	scanf("%u", &num);
	printf("you entered num = %u\n", num);
	number_of_1_bite(num);
}

void number_of_1_bite(unsigned int n){
	char result[sizeof(n)*32];
	short quantity = 0;
	for (size_t i = 0; i < sizeof(n) * 8; i++){
		int res = n & (1 << i);
		if (res > 0){
			quantity++;
			printf("1");
			//result[31 - i] = "1";
		}
		else printf("0");//result[31 - i] = "0";
		     
	}
	//printf("result = %c\n", result);
	printf("\nquantity = %d\n", quantity);
	printf("result of task = %d", quantity % 2);

}
