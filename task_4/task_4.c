#include <stdio.h>

struct abonent {
	char name[10];
	char second_name[10];
	char tel[10];
};

void showMenu(int num){
	printf("\n\nHello! Welcome to our subscriber List, current number of subscribers = %d\n", num);
	printf("Choose one:\n\t1.Add subscriber\n\t2.Delete subscriber\n\t");
	printf("3.Find subscriber by name\n\t4.Show subscriber List\n\t5.Exit\n");
}

void add_subscr(struct abonent *List, int num){
	printf("Print name, second name and tel\n");
	char name[10] = "0", second[10] = "0", tel[10] = "0";
	scanf("%s", name);
	scanf("%s", second);
	scanf("%s", tel);
	for (size_t i = 0; i < sizeof(name)/sizeof(name[0]); i++){
		List[num].name[i] = name[i];
		List[num].second_name[i] = second[i];
		List[num].tel[i] = tel[i];
	}
}

void del_subscr(struct abonent *List){
	int num = 0;
	printf("Enter number of abonent to delete\n");
	scanf("%d", &num);
	for (size_t i = 0; i < sizeof(List[0].name)/sizeof(List[0].name[0]); i++){
		List[num].name[i] = '0';
		List[num].second_name[i] = '0';
		List[num].tel[i] = '0';
	}
}

void print_str(char *, size_t);
void print_subscr(struct abonent *List, int num){
	printf("%d. {", num);
	print_str(List[num].name, sizeof(List[num].name));
	printf(", ");
	print_str(List[num].second_name, sizeof(List[num].second_name));
	printf(", ");
	print_str(List[num].tel, sizeof(List[num].tel));
	printf("}\n");
}

void find_subscr(struct abonent *List, int num){
	char find[10] = "0000";
	printf("Enter name of abonent for search\n");
	scanf("%s", find);
	for (int i = 0; i < num; i++){
		for (size_t ii = 0; sizeof(List[i].name)/sizeof(List[i].name[0]); ii++){
			if (List[i].name[ii] != find[ii]){
				//printf("%c != %c in i = %d, ii = %lu\n", List[i].name[ii], find[ii], i, ii);
				break;
			}
			else if (find[ii] == '\0' && List[i].name[ii] == '\0'){
				print_subscr(List, i);
				break;
			}
			//else printf("%c == %c in i = %d, ii = %lu\n", List[i].name[ii], find[ii], i, ii);
			
		}
	}
}

void show_all_subscr(struct abonent *List, int num){
	printf("----------start of list--------------\n");
	for (int i = 0; i < num; i++){
		print_subscr(List, i);
	} 
	printf("----------end of list--------------\n");
}

void print_str(char *string, size_t size){
	for (int i = 0; i < size/sizeof(char); i++){
		printf("%c", *string);
		string++;
	}
}	

int main(){
	int choice = 0;
	int cur_num = 0;
	struct abonent List[100];
	for (;;){
		showMenu(cur_num);
		scanf("%d", &choice);
		switch (choice){
			case 5:
				goto Exit;
			case 1:
				if (cur_num == 100) printf("List is overloaded\n");
				else{
					add_subscr(List, cur_num);
					cur_num++;
				}
				break;
			case 2:
				del_subscr(List);
				break;
			case 3:
				find_subscr(List, cur_num);
				break;
			case 4:
				show_all_subscr(List, cur_num);
				break;
		}
	}
	Exit:
		printf("Exit choosed\n");
		return 0;
	return 0;
}
