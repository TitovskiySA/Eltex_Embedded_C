#include <stdio.h>

int main(){
	int num;//user will enter this
	printf("Print some number (positive or negative)   ");
	scanf("%d", &num);//ask number
	char result[sizeof(num) * 8];//here will be result
	int quantity = 0; // quantity of 1 in result
	for (size_t i = 0; i < sizeof(result)/sizeof(result[0]); i++)
	{
		int res = num & (1 << i-1);//checking bites
		if (res > 0) quantity++;
		else continue;
		//printf("%c", result[sizeof(result) - i]);		
	}
	printf("Quantity of 1 = %d\n", quantity);
	
	return 0;
}
