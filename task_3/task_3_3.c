#include <stdio.h>

int main(){
	int massive[10] = {1,2,3,4,5,6,7,8,9,10};
	int *p_massive = massive;
	for (size_t i = 0; i < sizeof(massive)/sizeof(massive[0]); i++)
	{
		printf("massive[%lu] = %d\n", i, *p_massive);
		p_massive++;
	}
	return 0;
}

