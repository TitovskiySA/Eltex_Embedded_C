#include <stdio.h>

int main(){
	long result = 0;//result will be here
	long multipl = 1;//for multiply in decimal system
	int num;//user will enter this
	printf("Print some number (>0)   ");
	scanf("%d", &num);

	for (int i = 0; i < 32; i++)
	{
		int res = num & (1 << i);//checking bites
		if (res > 0) //if it is not 0 it must be 1
		{
			result = result + multipl;
		}
		multipl = multipl * 10;//increasing multipl
	}
	printf("result is = %ld\n", result);
	
	
	return 0;
}
