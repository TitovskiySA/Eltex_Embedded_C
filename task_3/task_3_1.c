#include <stdio.h>

int main(){
	int N = 0;
	int exch_N = 0;
	printf("Enter some int N?\n");
	scanf("%d", &N);
	printf("\nNow enter some number exch_N (less then 255) for exchange 3 byte of N\n");
	scanf("%d", &exch_N);
	if (exch_N > 255){
		printf("Your exch_N more than 255, please restart script...\n");
		return 0;
	}
	char *N_ptr = &N;
	char *exch_ptr = &exch_N;

	printf("\nBytes of N\n%d\n", *N_ptr);
        printf("%d\n", *(N_ptr + 1));
        printf("%d\n", *(N_ptr + 2));
        printf("%d\n", *(N_ptr + 3));

	printf("\nBytes of exch_N\n%d\n", *exch_ptr);
        printf("%d\n", *(exch_ptr + 1));
        printf("%d\n", *(exch_ptr + 2));
        printf("%d\n", *(exch_ptr + 3));


	*(N_ptr + 2) = *(exch_ptr);
	
	printf("\nBytes of new N\n");
	printf("%d\n", *N_ptr);
	printf("%d\n", *(N_ptr + 1));
	printf("%d\n", *(N_ptr + 2));
	printf("%d\n", *(N_ptr + 3));

	printf("\nNow your N = %d\n", N);

	return 0;
}
