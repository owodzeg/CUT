#include "printer.h"
#include "analyzer.h"
#include "buffer.h"
#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>

void *print_data(void* arg)
{
    double* analyzerData;
    unsigned long numcores;

    sleep(3);

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

            mvprintw(1+i, 0, "%lu", i);
            mvprintw(1+i, 11, "%f%%", analyzerData[i]);
        }

        refresh();

        usleep(100000);
    }
}
