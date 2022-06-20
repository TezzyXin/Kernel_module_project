#include "io_operations.c"

#define MAX_BUFFER	1024
#define COUNTER_FILE	"/proc/sykom/rj9del3"

int main(void)
{

	int cnt = open(COUNTER_FILE, O_RDWR);
	if(cnt < 0)
	{
		printf("Cannot open %s, error no. %d\n", COUNTER_FILE, errno);
		exit(1);
	}
	printf("Testing the counter:\n");
	read_op(COUNTER_FILE);
	sleep(1);
	printf("After reseting the counter by writing to it:\n");
	write_op(COUNTER_FILE, 1u << 5);
	read_op(COUNTER_FILE);
	sleep(2);
	printf("After waiting 3s:\n");
	read_op(COUNTER_FILE);

	return 0;
}
