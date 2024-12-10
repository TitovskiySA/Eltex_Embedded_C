#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
short str_len(char *str){
	short end = 0;
        //printf("Entered string = %s\n", str);
        for (int i = 0; i < MAXSIZE; i++){
                if (*str == '\n'){
			*str = '\0';
                        //printf("\nlengh founded %d\n", end);
                        break;
                }
                else{
                        //printf("%c", *str);
                        end++;
                        str++;
                }
        }
	return end;
}

int main(){
	char *massive = malloc(MAXSIZE);
	if (massive == NULL){
		printf("Error malloc for massive\n");
		return 1;
	}
	char *find = malloc(MAXSIZE);
	if (find == NULL){
		printf("Error malloc for find\n");
		return 1;
	}
	char *massive_p = massive;
	char *begin_massive = massive;
	char *find_p = find;
	char *begin_find = find;
	printf("Enter some string not more than 100 symbols\n");
	fgets(massive, MAXSIZE, stdin);
	printf("Enter some substring which we will find\n");
	fgets(find, MAXSIZE, stdin);
	printf("Entered massive = %s, len = %d\n", massive, str_len(massive));
	printf("Entered find = %s, len = %d\n", find, str_len(find));


	for (short i = 0; i < str_len(massive); i++)
	{
		//if (*massive_p == '\0') goto fault;
		if (*massive == '\0'){
			free(begin_massive);
			free(begin_find);
			goto fault;
		}
		for (short ii = 0; ii < str_len(find_p); ii++)
		{	
			//printf ("comp str %c with substr %c\n", *massive, *find);
			if (*find == '\0')
			{
				printf("Substring starts in pos = %d\n", i);
				free(begin_massive);
				free(begin_find);
				goto success;
			}
			if (*find == *massive)
			{	
				//printf("%c equal in num = %d\n", *find, i);
				find++;//if success go to compare text element
				massive++;
			}
				
			else 
			{
				printf("%c (str) not equal %c (substr) in num = %d\n", *massive, *find, i);
				massive = massive_p;//if not success we must return to our element from main cycle
				find = find_p;//if not success we must return to first element of substring
				break;
			} 
		}
		massive++;
		massive_p++;
	}
	
	fault:
		printf("Your substing was not founded\n");
		return 0;
	success:
		printf("success founded\n");
		return 0;

//	return 0;
}




