#include "io_operations.c"

#define AXIS_2_FILE	"/proc/sykom/rj9del2"

int main(void)
{
	int ax_2 = open(AXIS_2_FILE, O_RDWR);
	if(ax_2 < 0)
	{
		printf("Cannot open %s, error no. %d\n", AXIS_2_FILE, errno);
		exit(1);
	}

	printf("Testing reading from axis #2:\n");
	for(int i = 0; i < 16; i++)
	{
		printf("\t");
		read_op(AXIS_2_FILE);
		sleep(5);
	}

	printf("Testing writing to axis #2:\n");
	for(unsigned int i = 0; i < 16; i++)
	{
		printf("\t");
		write_op(AXIS_2_FILE, i << 21);
		sleep(2);
	}

	return 0;
}
