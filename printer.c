#include "printer.h"
#include "analyzer.h"
#include "buffer.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int printer_running;

void *print_data(void* arg)
{
    double* analyzerData;
    long numcores;

    struct timespec t;

    (void)arg; //to shut -wunused-parameter warning
    numcores = sysconf(_SC_NPROCESSORS_ONLN);
    printer_running = 1;

    //sleep(1);

    for(;;)
    {
        analyzerData = retrieve_data();

        if(analyzerData != NULL)
        {
            printf("\033[1;1H\033[2J"); //clear the screen for aesthetics
            printf("CORE ID\t\tUSAGE\n");
            for(long i=0; i<numcores; i++)
            {
                printf("%lu\t\t%f%%\n", i, analyzerData[i]);
            }
        }
        else
        {
            printf("Printer thread is waiting for data\n");
        }

        t.tv_sec = 1;
        t.tv_nsec = 0;
        nanosleep(&t, NULL);

        if(printer_running == 0)
        {
            return 0;
        }
    }
}

void printer_exit(void)
{
    printer_running = 0;
    return;
}
