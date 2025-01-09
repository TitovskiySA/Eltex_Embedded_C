#include <stdio.h>

void set_bit(unsigned int*, short);
void reset_bit(unsigned int*, short);
void invert_bit(unsigned int*, short);
void show_bin(unsigned int);

int main(){
	unsigned int num = 0;
	short bitnum = 0;
	printf("Enter some num  ");
	scanf("%u", &num);
	printf("you entered num = %u\n", num);
	show_bin(num);
	printf("Enter number of bit to change  ");
	scanf("%hu", &bitnum);

	//set bit
	set_bit(&num, bitnum);
	show_bin(num);

	//reset bit
	reset_bit(&num, bitnum);
	show_bin(num);

	//invert_bit
	invert_bit(&num, bitnum);
	show_bin(num);

	return 0;
}

void show_bin(unsigned int num){
	printf("now your bin:\n");
	for (size_t i = 0; i < sizeof(num) * 8; i++){
		int res = num & (1 << i);
		if (res > 0) printf("1");
		else printf("0");}
	printf("\n");
	return;
}
	
void set_bit(unsigned int* num_p, short bit){
	unsigned int mask = 1 << bit;
	*num_p = *num_p | mask;
	printf("set bit on num %hu \n", bit);
	return;
}

void reset_bit(unsigned int* num_p, short bit){
	unsigned int mask = ~(1 << bit);
	*num_p = *num_p & mask;
	printf("reset bit on num %hu \n", bit);
	return;
}

void invert_bit(unsigned int* num_p, short bit){
	int res = *num_p & (1 << bit);
	if (res > 0){
		reset_bit(num_p, bit);
		printf("inverted bit on num %hu from 1 to 0\n", bit);
	} else {
		set_bit(num_p, bit);
		printf("inverted bit on num %hu from 0 to 1\n", bit);
	}
	return;
}
	

















