#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "reader.h"
#include "analyzer.h"
#include "printer.h"

static void quit_program(int _)
{
	(void)_;

	reader_exit();
	analyzer_exit();
	printer_exit();
}

int main()
{
	pthread_t reader_thread;
	pthread_t analyzer_thread;
	pthread_t printer_thread;
	int result;
	long numcores;

	signal(SIGINT, quit_program);
	signal(SIGTERM, quit_program);

    numcores = sysconf(_SC_NPROCESSORS_ONLN);

	result = pthread_create(&reader_thread, NULL, read_proc, NULL);
	if(result != 0)
	{
		printf("Couldn't create reader thread\n");
		return 1;
	}

	result = pthread_create(&printer_thread, NULL, print_data, &numcores);
	if(result != 0)
	{
		printf("Couldn't create printer thread\n");
		return 1;
	}

	result = pthread_create(&analyzer_thread, NULL, process_data, &numcores);
	if(result != 0)
	{
		printf("Couldn't create analyzer thread\n");
		return 1;
	}

	pthread_join(printer_thread, NULL);
	pthread_join(reader_thread, NULL);
	pthread_join(analyzer_thread, NULL);

	printf("All threads have finished execution. Quitting program.\n");

	return 0;
}
