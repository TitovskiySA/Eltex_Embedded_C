#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

void fill_massive(int *massive, size_t size, int elem)
{
	for (size_t i = 0; i < size; i++)
	{
		*massive = elem + (int )i;// changing value on *massive
		//printf("creating massive element %zu = %d\n", i, *massive);
		massive = massive + 1;//change pointer on one size
	}
}

void print_massive(int *massive, size_t size, int N)
{
	for (size_t i = 0; i < size; i++)
        {
		if (i % N == 0) printf("\n");
                printf("%d ", *massive);
		massive = massive + 1;//change pointer on one size
        }
	printf("\n");

}

int main()
{	
	int N; //size of matrix
	int elem = 1; //first elem of matrix
	printf("Enter N?    ");
	scanf("%d", &N);
	int massive[N*N];
	printf("size of massive = %zu\n", sizeof(massive)/sizeof(massive[0]));
	fill_massive(massive, sizeof(massive)/sizeof(massive[0]), elem);
	print_massive(massive, sizeof(massive)/sizeof(massive[0]), N);
	return 0;
}
