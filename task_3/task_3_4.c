#include <stdio.h>

int main(){
	char massive[50] = "0000", find[50] = "0000";
	char *massive_p = massive, *padd_p = massive;
	char *find_p = find, *padd_find = find;
	printf("Enter some string not more than 50 symbols\n");
	scanf("%s", massive);
	printf("Enter some substring which we will find\n");
	scanf("%s", find);
	int Num_founded = 0;

	for (size_t i = 0; i < sizeof(massive)/sizeof(massive[0]); i++)
	{
		if (*massive_p == '\0') goto fault;
		for (size_t ii = 0; ii < sizeof(find)/sizeof(find[0]); ii++)
		{	
			//printf ("comp str %c with susctr %c\n", *massive_p, *find_p);
			if (*find_p == '\0')
			{
				printf("Substring starts in pos = %lu\n", i);
				goto success;
			}
			if (*find_p == *massive_p)
			{	
			//	printf("%c equal in num = %lu\n", *find_p, i);
				find_p++;//if success go to compare text element
				massive_p++;
			}
				
			else 
			{
			//	printf("%c (str) not equal %c (substr) in num = %lu\n", *massive_p, *find_p, i);
				massive_p = padd_p;//if not success we must return to our element from main cycle
				find_p = padd_find;//if not success we must return to first element of substring
				break;
			} 
		}
		massive_p++;
		padd_p++;
	}
	
	fault:
		printf("Your substing was not founded\n");
		return 0;
	success:
		printf("success founded\n");
		return 0;
}

