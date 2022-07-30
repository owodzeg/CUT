#include <stdio.h>
#include <pthread.h>
#include "reader.h"

int main()
{
	pthread_t reader_thread;
	int result = 0;

	printf("Hello, %s!\n", "World");

	result = pthread_create(&reader_thread, NULL, read_proc, NULL);
	if(result != 0)
	{
		printf("Couldn't create thread\n");
	}

	pthread_join(reader_thread, NULL);

	return 0;
}
