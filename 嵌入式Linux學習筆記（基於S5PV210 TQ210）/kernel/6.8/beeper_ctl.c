#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/fcntl.h>

int main(int argc, char *argv[])
{
	int fd = -1;
	int num;
	size_t wb;
	char name[20];
	struct input_event ev;
	
	if ((fd = open("/dev/input/event0", O_RDWR)) < 0)  //open device
	{
		perror("open error");
		exit(1);
	}

	printf("Please enter the value(Hz) (0 is stop) :\n");
	while (1)
	{
		ev.type = EV_SND;
		ev.code = SND_TONE;

		scanf("%d", &ev.value);

		printf("freq = %d Hz\n", ev.value);
		
		if (ev.value == 0)
			sleep(2);
		wb = write(fd, &ev, sizeof(struct input_event));
		if (wb < sizeof(struct input_event))
		{
			perror("write error");
			exit(1);
		}
		if (ev.value == 0)
			break;
	}
	close(fd);
	
	return 0;
}
