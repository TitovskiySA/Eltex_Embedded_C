#include <stdio.h>

int main()
{	
	int N; //size of matrix
	printf("Enter N?    ");
	scanf("%d", &N);
	int massive[N][N];
	int a = 0; // first number in row = 0, col = 0
	for (int times = 0; times < N/2; times++)
	{
		//first row
		for (int col = 0 + times; col < N - 1 - times; col++)
		{
			//printf("first row[%d][%d]\n", times, col);
			massive[times][col] = a;
			a++;
		}
		//last col
		for (int row = 0 + times; row < N - 1 - times; row++)
		{
			//printf("last col[%d][%d]\n", row, N - 1 - times);
			massive[row][N - 1 - times] = a;
			a++;
		}
		//last row
		for (int col = N - 1 - times; col > times; col--)
                {
                        //printf("last row[%d][%d]\n", N - 1 - times, col);
                        massive[N - 1 - times][col] = a;
                        a++;
                }
		//first col
		for (int row = N - 1 - times; row > times; row--)
                {
                        //printf("first col[%d][%d]\n", row, times);
                        massive[row][times] = a;
                        a++;
                }
	}
	//if not divide on 2, then insert center element
	if (N % 2 != 0) massive[N/2][N/2] = a;
	
	//printing massive
	for (int row = 0; row < N; row++)
	{
		for (int col = 0; col < N; col++)
		{
			printf("%d\t", massive[row][col]);
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}
