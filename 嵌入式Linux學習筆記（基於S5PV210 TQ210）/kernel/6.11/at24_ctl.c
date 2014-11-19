#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/* i2c_usr_test <i2c-0> r addr
 * i2c_usr_test <i2c-0> w addr val
 */

void print_usage(char *file)
{
	printf("%s <i2c-0>  r addr\n", file);
	printf("%s <i2c-0>  w addr val\n", file);
}

int main(int argc, char **argv)
{
	int fd;
	unsigned char addr, data;
	int dev_addr;
	char bin_file_name[128];
	
	if ((argc != 4) && (argc != 5))
	{
		print_usage(argv[0]);
		return -1;
	}

	sprintf(bin_file_name, "/sys/bus/i2c/devices/%s/0-0050/eeprom", argv[1]);
	
	fd = open(bin_file_name, O_RDWR);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	addr = strtoul(argv[3], NULL, 0);
	lseek(fd, addr, SEEK_SET);
	
	if (strcmp(argv[2], "r") == 0)
	{	
		read(fd, &data, 1);
		printf("data:%#x\n", data);
	}
	else if ((strcmp(argv[2], "w") == 0) && (argc == 5))
	{
		data = strtoul(argv[4], NULL, 0);
		write(fd, &data, 1);
	}
	else
	{
		print_usage(argv[0]);
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

