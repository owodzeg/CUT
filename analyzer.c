#include "analyzer.h"
#include <unistd.h>
#include <stdio.h>

struct CoreData* old_core_data = NULL;
struct CoreData* new_core_data = NULL;

void* process_data(void* arg)
{
    sleep(2);

    unsigned long numcores = 0;
    numcores = get_num_cores();
    
    struct CoreData* tmp;

    long int prevIdle;
    long int curIdle;

    long int prevNonIdle;
    long int curNonIdle;

    long int prevTotal;
    long int curTotal;

    long int totalDiff;
    long int idleDiff;

    double percentage;

    if(old_core_data == NULL)
        old_core_data = malloc(sizeof(struct CoreData) * numcores);

    if(new_core_data == NULL)
        new_core_data = malloc(sizeof(struct CoreData) * numcores);

    for(;;)
    {
        tmp = load_from_buffer();
        
        for(unsigned long i=0; i<numcores; i++)
        {
            new_core_data[i].core_id = tmp[i].core_id;
            new_core_data[i].user = tmp[i].user;
            new_core_data[i].nice = tmp[i].nice;
            new_core_data[i].system = tmp[i].system;
            new_core_data[i].idle = tmp[i].idle;
            new_core_data[i].iowait = tmp[i].iowait;
            new_core_data[i].irq = tmp[i].irq;
            new_core_data[i].softirq = tmp[i].softirq;
            new_core_data[i].steal = tmp[i].steal;
        
            // logic
            //printf("core %lu user %ld vs %ld\n", i+1, new_core_data[i].user, old_core_data[i].user);

            prevIdle = old_core_data[i].idle + old_core_data[i].iowait;
            curIdle = new_core_data[i].idle + new_core_data[i].iowait;

            prevNonIdle = old_core_data[i].user + old_core_data[i].nice + old_core_data[i].system + old_core_data[i].irq + old_core_data[i].softirq + old_core_data[i].steal;
            curNonIdle = new_core_data[i].user + new_core_data[i].nice + new_core_data[i].system + new_core_data[i].irq + new_core_data[i].softirq + new_core_data[i].steal;

            prevTotal = prevIdle + prevNonIdle;
            curTotal = curIdle + curNonIdle;

            totalDiff = curTotal - prevTotal;
            idleDiff = curIdle - prevIdle;

            percentage = (double)(totalDiff - idleDiff) / (double)totalDiff * 100.0;

            printf("CPU core: %lu, usage: %f%%, totald: %ld, idled: %ld\n", i, percentage, totald, idled);

            old_core_data[i].core_id = new_core_data[i].core_id;
            old_core_data[i].user = new_core_data[i].user;
            old_core_data[i].nice = new_core_data[i].nice;
            old_core_data[i].system = new_core_data[i].system;
            old_core_data[i].idle = new_core_data[i].idle;
            old_core_data[i].iowait = new_core_data[i].iowait;
            old_core_data[i].irq = new_core_data[i].irq;
            old_core_data[i].softirq = new_core_data[i].softirq;
            old_core_data[i].steal = new_core_data[i].steal;
        }

        sleep(1);
    }
}
