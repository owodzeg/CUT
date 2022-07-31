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
    unsigned long numcores;

    struct timespec t;

    sleep(1); //need to wait for reader and analyzer to kick in

    initscr();

    for(;;)
    {
        analyzerData = retrieve_data();
        numcores = get_num_cores();
        
        mvprintw(0, 0, "CORE ID    USAGE");

        for(unsigned long i=0; i<numcores; i++)
        {
            move(1+i, 0);
            clrtoeol();

            mvprintw(1+(int)i, 0, "%lu", i);
            mvprintw(1+(int)i, 11, "%f%%", analyzerData[i]);
        }

        refresh();


        t.tv_sec = 0;
        t.tv_nsec = 100000000L;
        nanosleep(&t, NULL);
    }
}
