//более удобным и гибким в работе мне показался FIFO
//этот механизм позволяет передавать тип переменных, отличный от char
//время передачи 1000 элементов примерно одинаково

titovskiysa@titovskiysa-Extensa-5630:~/Eltex/task_10$ gcc FIFO.c 
titovskiysa@titovskiysa-Extensa-5630:~/Eltex/task_10$ ./a.out 
writer process started
Message sent: some message_0 (17 bytes)
...
Message received: some message_999 (17 bytes)
Message received: end (4 bytes)
received end message
parent finished
elapsed on FIFO 0.000000 sec, 35443418.000000 nsec


itovskiysa@titovskiysa-Extensa-5630:~/Eltex/task_10$ gcc MQUEUE.c 
titovskiysa@titovskiysa-Extensa-5630:~/Eltex/task_10$ ./a.out 
sended to queue some message_0
sended to queue some message_1
sended to queue some message_2
sended to queue some message_3
sended to queue some message_4
sended to queue some message_5
sended to queue some message_6
sended to queue some message_7
sended to queue some message_8
sended to queue some message_9
received = some message_0(17 bytes)
received = some message_1(17 bytes)
received = some message_2(17 bytes)
...
received = some message_999(17 bytes)
received = end(4 bytes)
end message from queue
elapsed on mq 0.000000 sec, 39575357.000000 nsec

