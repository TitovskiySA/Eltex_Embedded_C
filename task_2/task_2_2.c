#include <stdio.h>


int read_massive(int *massive)
{
	int size = 0;
	int member;
	printf("Enter int massive's members (not more then 30), if you want to stop enter 0\n");
	for (;;)
	{
		printf("Enter another massive member =  ");
		scanf("%d", &member);
		if (member == 0) break;
		*massive = member;
		massive = massive + 1;
		size = size + 1;
	}
	return size;
}

int main()
{	
	int massive[30]; //our massive
	int *ptr_massive = massive;
	int size = read_massive(massive); //find real number of members
	printf("Result\n");
	for (int i = size - 1; i >= 0; i--)
	{
		printf("%d ", *(ptr_massive + i));
	}
	return 0;
}
