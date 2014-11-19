#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdbool.h>
#include <linux/tiocl.h>
#include <asm-generic/ioctls.h>

int main(int argc, char **argv)
{
	int ret;
	int blank, interval;
	int fd;
	char buf[10];
	
	fd = open("/dev/tty1", O_RDWR);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	
	if (argc == 2)
	{
		if (strcmp(argv[1], "on") == 0)
		{
			blank = TIOCL_UNBLANKSCREEN;
			printf("unblank screen\n");
		}
		else
		{
			blank = TIOCL_BLANKSCREEN;
			printf("blank screen\n");
		}
		ret = ioctl(fd, TIOCLINUX, &blank);
		if (ret < 0)
		{
			perror("ioctl");
			close(fd);
			exit(1);
		}
		
	}
	else if (argc == 3)
	{
		interval = atoi(argv[2]);
		sprintf(buf, "\033[9;%d]", interval);
		ret = write(fd, buf, strlen(buf) + 1);
		if (ret < 0)
		{
			perror("write");
			close(fd);
			exit(1);
		}
	}
	else
	{
		printf("Usage:%s on [n]\n"
				"%s off", argv[0]);
	}
	
	close(fd);
	return 0;
}
