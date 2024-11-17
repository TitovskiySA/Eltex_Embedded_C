#include <stdio.h>

int main(){
	int num;//user will enter this
	printf("Print some number (positive or negative)   ");
	scanf("%d", &num);//ask number
	char result[sizeof(num) * 8];//here will be result
//	printf("sizeof int = %zu\n", sizeof(num) * 8);

	for (size_t i = 0; i < sizeof(result)/sizeof(result[0]); i++)
	{
		int res = num & (1 << i-1);//checking bites
		if (res > 0) result[sizeof(result) - i] = '1';
		else result[sizeof(result) - i] = '0';
		//printf("%c", result[sizeof(result) - i]);		
	}
	//printf("\n");
	int begin;//find first 0 in positive number
	for (size_t p = 0; p < sizeof(result)/sizeof(result[0]); p++)
	{
		if (num >= 0)
		{
			if (result [p] == '1')
			{
				begin = p;
				break; 
			}
		}
		else begin = 1;
	}
	for (size_t u = begin; u < sizeof(result)/sizeof(result[0]); u++)
	{
		printf("%c", result[u]); //printing result
	}
	printf("\n");
	return 0;
}
