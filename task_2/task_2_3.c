#include <stdio.h>

void fill_massive(int *massive, size_t size, int N)
{	
	int i = 0;
	int row;
	int col;
	for (;;)
	{
		if (i == (int) size) break;
		row = i / N;
		col = i % N;
		//printf("current row, col = %d, %d\n", row, col);
		if (row + col < N - 1) *massive = 0;
		else *massive = 1;
		massive = massive + 1; 
		i++;
		 
	}
}

void print_massive(int *massive, size_t size, int N)
{
	for (size_t i = 0; i < size; i++)
        {
		if (i % N == 0) printf("\n");
		printf("%d", *massive);
		massive = massive + 1;
        }
	printf("\n");

}

int main()
{	
	int N; //size of matrix
	printf("Enter N?    ");
	scanf("%d", &N);
	int massive[N*N];
	printf("size of massive = %zu\n", sizeof(massive)/sizeof(massive[0]));
	fill_massive(massive, sizeof(massive)/sizeof(massive[0]), N);
	print_massive(massive, sizeof(massive)/sizeof(massive[0]), N);
	return 0;
}
