#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "analyzer.h"
#include "printer.h"
#include "buffer.h"

static int artificial_cores;
static int artificial_running;

static void quit_program(int _)
{
	(void)_;

	artificial_running = 0;
	analyzer_exit();
	printer_exit();
}

void* artificial_reader(void*);

void* artificial_reader(void* arg)
{
    struct CoreData data;
    int user;
    int idle;
    struct timespec t;
	(void)arg;

    user = 0;
    idle = 0;

	for(;;)
	{
		for(int i=0; i<artificial_cores; i++)
		{
            user += rand() % 100;
            idle += rand() % 100;

			data.core_id = (unsigned long)i;
			data.user = user;
			data.nice = 0;
			data.system = 0;
			data.idle = idle;
			data.iowait = 0;
			data.irq = 0;
			data.softirq = 0;
			data.steal = 0;

			save_to_buffer(data);
		}

        t.tv_nsec = 500000000L;
        t.tv_sec = 0;
        nanosleep(&t, NULL);

        if(artificial_running == 0)
        {
            return 0;
        }
	}
}

int main(int argc, char *argv[])
{
	pthread_t reader_thread;
	pthread_t analyzer_thread;
	pthread_t printer_thread;
	int result;
    long numcores;
    long seed;
    time_t tt;

    if(argc < 2)
    {   
        printf("create artificial cores to test expandability of the tool\nusage:\n\t./test numOfCores\n");
        return 1;
    }

    seed = time(&tt);
    srand((unsigned int)seed);

    artificial_cores = atoi(argv[1]);
	artificial_running = 1;
    numcores = (long)artificial_cores;

	signal(SIGINT, quit_program);

	result = pthread_create(&reader_thread, NULL, artificial_reader, NULL);
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
