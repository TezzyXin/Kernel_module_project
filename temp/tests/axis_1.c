#include "io_operations.c"
#define MAX_BUFFER	1024
#define AXIS_1_FILE	"/proc/sykom/rj9del1"


int main(void)
{
	int ax_1 = open(AXIS_1_FILE, O_RDWR);
	if(ax_1 < 0)
	{
		printf("Cannot open %s, error no. %d\n", AXIS_1_FILE, errno);
		exit(1);
	}

	printf("Testing reading from axis #1:\n");
	for(int i = 0; i < 16; i++)
	{
		printf("\t");
		read_op(AXIS_1_FILE);
		sleep(5);
	}

	printf("Testing writing to axis #1:\n");
	for(unsigned int i = 0; i < 16; i++)
	{
		printf("\t");
		write_op(AXIS_1_FILE, i << 9);
		sleep(2);
	}

	return 0;
}
