#include <stdio.h>
#include <pthread.h>
#include "reader.h"
#include "analyzer.h"

int main()
{
	pthread_t reader_thread;
	pthread_t analyzer_thread;
	int result = 0;

	printf("Hello, %s!\n", "World");

	result = pthread_create(&reader_thread, NULL, read_proc, NULL);
	if(result != 0)
	{
		printf("Couldn't create thread\n");
	}

	result = pthread_create(&analyzer_thread, NULL, process_data, NULL);
	if(result != 0)
	{
		printf("Couldn't create thread\n");
	}

	pthread_join(reader_thread, NULL);
	pthread_join(analyzer_thread, NULL);

	return 0;
}
