#include <stdio.h>
#include <pthread.h>
#include "reader.h"
#include "analyzer.h"
#include "printer.h"

int main()
{
	pthread_t reader_thread;
	pthread_t analyzer_thread;
	pthread_t printer_thread;
	int result = 0;

	result = pthread_create(&reader_thread, NULL, read_proc, NULL);
	if(result != 0)
	{
		printf("Couldn't create thread\n");
	}

	result = pthread_create(&printer_thread, NULL, print_data, NULL);
	if(result != 0)
	{
		printf("Couldn't create thread\n");
	}

	result = pthread_create(&analyzer_thread, NULL, process_data, NULL);
	if(result != 0)
	{
		printf("Couldn't create thread\n");
	}

	pthread_join(printer_thread, NULL);
	pthread_join(reader_thread, NULL);
	pthread_join(analyzer_thread, NULL);

	return 0;
}
