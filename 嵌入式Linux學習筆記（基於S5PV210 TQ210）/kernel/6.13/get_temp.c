#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DS18B20_DEV		"/sys/devices/w1_bus_master1/28-000004de67f8/w1_slave"

/* return 0 on success， or -1 on erro */
int get_temp(float *value, const char *dev)
{
	int ret;
	FILE *fp;
	char buf1[100], buf2[100];
	
	fp = fopen(dev, "r");
	if (fp == NULL)
	{
		perror("fopen");
		return -1;
	}
	
	fgets(buf1, 100, fp);
	sscanf(buf1, "%*[^:]: crc=%[^ ]%s\n", buf2);
	if (strcmp(buf2, "YES") == 0)
	{
		fgets(buf1, 100, fp);
		sscanf(buf1, "%*[^t]t=%s\n", buf2);
		*value = atoi(buf2) * 1.0 / 1000;
		ret = 0;
	}
	else
	{
		fprintf(stderr, "CRC error\n");
		ret = -1;
	}

	fclose(fp);
	return ret;
}

int main(int argc, char **argv)
{
	float value;
	if (get_temp(&value, DS18B20_DEV) == 0)
		printf("%.3f °C\n", value);
	return 0;
}
