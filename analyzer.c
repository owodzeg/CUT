#include "analyzer.h"
#include <unistd.h>
#include <stdio.h>

struct CoreData* old_core_data = NULL;
struct CoreData* new_core_data = NULL;

void* process_data(void* arg)
{
    unsigned long numcores;
    
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

    int** percentagesStored; //we store many percentage data to get an average per second

    int refresh_rate; //how many refreshes per second

    double average;

    int* averagesStored; //storage for final calculation

    sleep(2); //wait to make sure we have an appropriate number of cores

    refresh_rate = 10;
    numcores = get_num_cores();

    percentagesStored = malloc(sizeof(int*) * numcores);
    if(!percentagesStored)
    {
        printf("Unable to allocate memory for percentagesStored\n");
    }
    else
    {
        for(unsigned long i=0; i<numcores; i++)
        {
            percentagesStored[i] = malloc(sizeof(double) * refresh_rate);

            if(!percentagesStored[i])
            {
                printf("Unable to allocate memory for percentagesStores[%lu]", i);
            }
        }
    }

    for(unsigned long c=0; c<numcores; c++)
    {
        for(int r=0; r<refresh_rate; r++)
        {
            percentagesStored[c][r] = 0.0;
        }
    }

    averagesStored = malloc(sizeof(double) * numcores);

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

            //printf("totalDiff: %ld, idleDiff: %d", totalDiff, idleDiff);

            if(totalDiff > 0)
            {
                percentage = (double)(totalDiff - idleDiff) / (double)totalDiff * 100.0;
            }
            else
            {
                percentage = 0;
            }

            //printf("Percentage: %f", percentage);

            for(int r=0; r<refresh_rate-1; r++)
            {
                percentagesStored[i][r] = percentagesStored[i][r+1];
            }

            percentagesStored[i][refresh_rate-1] = percentage;

            average = 0.0;

            for(int r=0; r<refresh_rate; r++)
            {
                average += percentagesStored[i][r];
            }

            //printf("Average value: %f", average);

            average = average / (double)refresh_rate;

            printf("CPU core: %lu, usage: %f%%\n", i, average);

            averagesStored[i] = average;

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

        usleep(1000000 / refresh_rate);
    }
}
