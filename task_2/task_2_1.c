#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

void fill_massive(int massive[], int elem)
{
	printf("size of massive in foo1 = %zu", sizeof(massive)/sizeof(massive[0]));
	for (size_t i = 0; i < sizeof(massive)/sizeof(massive[0]); i++)
	{
		massive[i] = elem + (int )i;
		printf("creating massive element %zu = %d\n", i, massive[i]);
	}
}

void print_massive(int massive[], int N)
{
	for (size_t i = 0; i < sizeof(massive)/sizeof(massive[0]); i++)
        {
		if ((i + 1) % N == 0) printf("\n");
                printf("%d", massive[i]);
        }

}

int main()
{	
	int N; //size of matrix
	int elem = 1; //first elem of matrix
	int *ptr; //pointer on massive
	printf("Enter N?    ");
	scanf("%d", &N);
	//int quantity = N * N; // number of elements
	ptr = (int*)malloc(N * N * sizeof(int)); // member for massive
	int massive[N*N];
//	int a* = malloc(N * N * sizeof(int));
	printf("size of massive = %zu\n", sizeof(massive)/sizeof(massive[0]));
	fill_massive(massive, elem);
	//print_massive(ptr, N);
	return 0;
}
