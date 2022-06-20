#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define MAX_BUFFER	1024

void read_op(const char *path)
{
	char buffer[MAX_BUFFER];
	int fd_in = open(path, O_RDONLY);
	if(fd_in < 0)
	{
		printf("Cannot open %s, error no. %d\n", path, errno);
		exit(1);
	}

	int n = read(fd_in, buffer, MAX_BUFFER);
	close(fd_in);

	if(n < 0)
	{
		printf("Cannot read value, error no. %d\n", errno);
		exit(1);
	}
	buffer[n] = '\0';
	printf("Read: 0x%s", buffer);
}


void write_op(const char *path, unsigned int val)
{
	char buffer[MAX_BUFFER];
	int fd_out = open(path, O_RDWR);
	if(fd_out < 0)
	{
		printf("Cannot open %s, error no. %d\n", path, errno);
		exit(1);
	}

	snprintf(buffer, MAX_BUFFER, "%x", val);
	int n = write(fd_out, buffer, strlen(buffer));
	close(fd_out);
	if(n < 0)
	{
		printf("Cannot write value to %s, error no. %d\n", path, errno);
		exit(1);
	}

	printf("Writing 0x%x to %s\n", val, path);
}
