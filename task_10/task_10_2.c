#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <time.h>

#define QPATH "/QPATH1" // Path to the queue
#define num 1000
#define MAXSIZE 1024

int main()
{
    mqd_t mq = mq_open("/mq", O_RDWR | O_CREAT, 0666, NULL);
    if (mq == -1) exit(1); 

    char mes[] = "adventure";
    int n = mq_send(mq, mes, sizeof(mes), 0);
    char mes2[] = "eightcharacters";
    n = mq_send(mq, mes2, sizeof(mes2), 0);
    if (n == -1) exit(1);

    struct mq_attr attr;
    int buflen;  
    char *buf;

    mq_getattr(mq, &attr);
    buflen = attr.mq_msgsize;
    buf = (char *) malloc(buflen);

    printf("buflen: %d\n", buflen);

    n = mq_receive(mq, (char *) buf, buflen, NULL);
    if (n == -1) { exit(1); }

    printf("%s\n",buf);

    n = mq_receive(mq, (char *) buf, buflen, NULL);
    if (n == -1) { exit(1); }

    printf("%s\n",buf);

    free(buf);
    mq_close(mq);
    return 0;
}