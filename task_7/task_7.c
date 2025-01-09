#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define BUFF 128

void enter_dynamic_string(char ** str_p){ //foo saves in this string
        char *str = (char*)malloc(sizeof(char) * 2); // creating pointer on our str
        int symb; // getchar returns int
        for (;;){
                symb = getchar();
                if (symb != '\n') break; // if read not empty symbol, it is first entered by user char
                //else printf("\nignoring some empty symb");
        }
        str[0] = symb; // this symbol will be first in our string
        str[1] = '\0'; // at the end must be this symbol - EOS
        int i = 1; // for next cycle
        for (;;){
                if (symb == '\n') break; // end of entered string
                str = realloc(str, (i + 2) * sizeof(char)); // memory for our string + ending symbol
                symb = getchar(); // enter one char
		if (symb == '\n') continue;
                str[i] = (char)symb; // writing our symbol in string
                str[i+1] = '\0'; // adding finishing symbol
                i++; // next cycle
        }
        *str_p = str; // save to entered pointer address of result string
        return;
}



int task(){
	//ask for namefile
	printf("Enter name of file\n");
	char* namefile;
	enter_dynamic_string(&namefile);
	printf("\nYou entered namefile = %s\n", namefile);
	//ask for txt
	printf("Enter txt_to_write:\n");
	char* txt_to_wr;
	enter_dynamic_string(&txt_to_wr);
	printf("\nYou entered txt_to_wr = %s", txt_to_wr);

	//open file for write
	int fd = open(namefile, O_WRONLY|O_CREAT, 0644);
	if (fd == -1){
		perror("open");
		return 1;
	}
	
	//making offset to end of file
	off_t pos_to_wr = lseek(fd, 0, SEEK_END);
	if (pos_to_wr == -1){
		perror("lseek");
		close(fd);
		return 1;
	}
	
	//write txt_to_wr to fd
	if (write(fd, txt_to_wr, strlen(txt_to_wr)) == -1){
		perror("write");
		close(fd);
		return 1;
	}
	printf("\nYou data were write in file\n");
	if (close(fd) == -1){
		perror("close1");
		return 1;
	}

	//open file  again
	fd = open(namefile, O_RDONLY);
	if (fd == -1){
		perror("open_rd");
		return 1;
	}
	//we don't need offset now, it's default at beginning
	//reading bytes from file in buffer and write it to terminal
	char buffer[BUFF] = {'0'};
	ssize_t bytes_read;

	while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0){
		if (bytes_read == -1){
			perror("read");
			close(fd);
			return 1;
		}
		buffer[bytes_read] = '\0';
		printf("%s", buffer);
	}

	if (close(fd) == -1){
		perror("close2");
		return 1;
	}
	
	printf("\n");
	free(namefile);
	free(txt_to_wr);
	return 0;				

}
	
int main(){
	task();
	return 0;
}
