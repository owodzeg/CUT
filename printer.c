#include "printer.h"
#include "analyzer.h"
#include "buffer.h"
#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>

void *print_data(void* arg)
{
    double* analyzerData;
    long numcores;

    struct timespec t;

    (void)arg; //to shut -wunused-parameter warning
    numcores = sysconf(_SC_NPROCESSORS_ONLN);

    //sleep(1);

    initscr();

    for(;;)
    {
        analyzerData = retrieve_data();
        mvprintw(0, 0, "CORE ID    USAGE");

        if(analyzerData != NULL)
        {
            for(long i=0; i<numcores; i++)
            {
                move(1+i, 0);
                clrtoeol();

                mvprintw(1+(int)i, 0, "%lu", i);
                mvprintw(1+(int)i, 11, "%f%%", analyzerData[i]);
            }
        }
        else
        {
            mvprintw(1, 0, "Printer thread is waiting for data");
        }

        refresh();

        t.tv_sec = 1;
        t.tv_nsec = 0;
        nanosleep(&t, NULL);
    }
}
