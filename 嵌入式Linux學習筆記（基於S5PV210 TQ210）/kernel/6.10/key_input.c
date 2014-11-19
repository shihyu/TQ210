#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/fcntl.h>
#include <string.h>
#include <signal.h>

int fd = -1;
struct input_event ev;

void sig_handle(int sig)
{
	int rc = -1;
		
	rc = read(fd, &ev, sizeof(struct input_event));
	if (rc < 0)
	{
		perror("read");
		return;
	}
	if (EV_KEY == ev.type)
	{
		if (1 == ev.value)
			printf("key %d, value:%d\n", ev.code, ev.value);
		else
			printf("key %d, value:%d\n", ev.code, ev.value);
	}
}

int main(int argc, char *argv[])
{
	size_t rb;
	int version;
	char name[20];
	
	int oflags = 0;
	
	if (argc != 2)
	{
		fprintf(stderr, "Usage:%s /dev/eventx\n", argv[0]);
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open error");
		exit(1);
	}
	
#if 0	
	if (ioctl(fd, EVIOCGNAME(sizeof(name)-1), name) < 0)
	{
		perror("getname error");
		exit(1);
	}
	printf("name=%s\n", name);
	
	if (ioctl(fd, EVIOCGVERSION, &version) < 0)
	{
		perror("getversion error");
		exit(1);
	}
	printf("version=0x%x\n", version);

	while(1)
	{
		rb = read(fd, &ev, sizeof(struct input_event));
		
		if (rb < (int)sizeof(struct input_event))
		{
			perror("read error");
			exit(1);
		}

		if (EV_KEY == ev.type)
		{
			if (1 == ev.value)
				printf("key %d is pressed\n", ev.code);
			else
				printf("key %d is releassed\n", ev.code);
		}
	}
#else
	signal(SIGIO, sig_handle);
	fcntl(fd, F_SETOWN, getpid());
	oflags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, oflags | FASYNC);
	while (1)
		sleep(1);
#endif
	close(fd);
	
	return 0;
}

