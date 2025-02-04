#ifndef IOCTL_TEST_H
#define IOCTL_TEST_H

struct mystruct{
	int read_counter;
	int write_counter;
	int some_setting;
	char device_name[64];
};

#define WR_VALUE _IOW('a', 'a', int *)
#define RD_VALUE _IOR('a', 'b', int *)
#define GREETER  _IOW('a', 'c', struct mystruct *)

#endif
