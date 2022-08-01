#include "analyzer.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>

struct CoreData* old_core_data;
struct CoreData* new_core_data;
double* averagesStored;
int analyzer_running = 1;

void* process_data(void* arg)
{
    long numcores;
    
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

    double** percentagesStored; //we store many percentage data to get an average per second

    int refresh_rate; //how many refreshes per second

    double average;

    struct timespec t;

    (void)arg; //to shut -wunused-parameter warning

    old_core_data = NULL;
    new_core_data = NULL;

    averagesStored = NULL;

    refresh_rate = 10;
    numcores = *((long *) arg);

    percentagesStored = malloc(sizeof(double*) * (unsigned long)numcores);
    if(!percentagesStored)
    {
        printf("Unable to allocate memory for percentagesStored\n");
    }
    else
    {
        for(long i=0; i<numcores; i++)
        {
            percentagesStored[i] = malloc(sizeof(double) * (unsigned long)refresh_rate);

            if(!percentagesStored[i])
            {
                printf("Unable to allocate memory for percentagesStores[%lu]", i);
            }
        }
    }

    for(long c=0; c<numcores; c++)
    {
        for(int r=0; r<refresh_rate; r++)
        {
            percentagesStored[c][r] = 0.0;
        }
    }

    pthread_mutex_lock(&analyzer_mutex);
    averagesStored = malloc(sizeof(double) * (unsigned long)numcores);
    pthread_mutex_unlock(&analyzer_mutex);

    if(old_core_data == NULL)
        old_core_data = malloc(sizeof(struct CoreData) * (unsigned long)numcores);

    if(new_core_data == NULL)
        new_core_data = malloc(sizeof(struct CoreData) * (unsigned long)numcores);

    for(;;)
    {
        pthread_mutex_lock(&buffer_mutex);
        tmp = load_from_buffer();
        pthread_mutex_unlock(&buffer_mutex);
        
        if(tmp != NULL)
        {
            for(long i=0; i<numcores; i++)
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

                //printf("CPU core: %lu, usage: %f%%\n", i, average);

                pthread_mutex_lock(&analyzer_mutex);
                averagesStored[i] = average;
                pthread_mutex_unlock(&analyzer_mutex);

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
        }

        t.tv_sec = 0;
        t.tv_nsec = 1000000000L / refresh_rate;
        nanosleep(&t, NULL);

        if(analyzer_running == 0)
        {   
            for(long i=0; i<numcores; i++)
            {
                free(percentagesStored[i]);
            }

            free(percentagesStored);      
            free(averagesStored);
            free(old_core_data);
            free(new_core_data);

            return 0;
        }
    }
}

double* retrieve_data()
{
    if(averagesStored != NULL)
    {
        return averagesStored;
    }
    else
    {
        return NULL;
    }
}

void analyzer_exit(void)
{
    analyzer_running = 0;
    return;
}
