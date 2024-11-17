#include <stdio.h>

int main(){
	int num, exch;//user will enter this
	printf("Print some number (positive or negative)   ");
	scanf("%d", &num);//ask number
	printf("\nNow print some number for exchange bites (less then 255)   ");
	scanf("%d", &exch);//ask exchange
	char result[sizeof(num) * 8];//here will be result
	char res_exch[sizeof(num) * 8 / 4];//for bites of exchange

	for (size_t i = 0; i < sizeof(result)/sizeof(result[0]); i++)
	{
		int res = num & (1 << i-1);//checking bites of num
		if (res > 0) result[sizeof(result) - i] = '1';
		else result[sizeof(result) - i] = '0';		
	}
	for (size_t i = 0; i < sizeof(res_exch)/sizeof(res_exch[0]); i++)
        {
		//printf("now cycle = %zu\n", i);
                int res2 = exch & (1 << i);//checking bites of exch
                if (res2 > 0) res_exch[7 - i] = '1';
                else res_exch[7 - i] = '0';
        }
	for (size_t i = 0; i <= sizeof(result)/sizeof(result[0]); i++)
        {
		if (16 <= i && i <= 23)
		{	
			result[i] = res_exch[i-16];
		}
        }
	printf("\nResult:\n");
        for (size_t i = 0; i < sizeof(result)/sizeof(result[0]); i++)
        {
                printf("%c", result[i]);                 
        }

	return 0;
}
