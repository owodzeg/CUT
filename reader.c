#include "reader.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void* read_proc(void* arg)
{
    for(;;)
    {
        FILE *fptr;
        char line[1024];

        char cpu[255];
        long int user=0, nice=0, system=0, idle=0, iowait=0, irq=0, softirq=0, steal=0;

        char head[3];
        unsigned long core = 0;

        struct CoreData data;
        struct timespec t;

        (void)arg; //to shut -wunused-parameter warning

        fptr = fopen("/proc/stat", "r");
        
        if(fptr == NULL)
        {
            //TODO: replace it with logger thread
            printf("Could not open file.\n");
        }

        while(fgets(line, 1024, fptr) != NULL) 
        {
            //printf("%s", line);
            sscanf(line, "%s %ld %ld %ld %ld %ld %ld %ld %ld", cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
            strncpy(head, cpu, 3);
            
            if(strcmp("cpu", head) == 0 && strlen(cpu) > 3)
            {
                core = strtoul(cpu+3, NULL, 10);
                //printf("cpu: %lu, user: %ld, nice: %ld, system: %ld, idle: %ld, iowait: %ld, irq: %ld, softirq: %ld, steal: %ld\n", core, user, nice, system, idle, iowait, irq, softirq, steal);

                data.core_id = core;
                data.user = user;
                data.nice = nice;
                data.system = system;
                data.idle = idle;
                data.iowait = iowait;
                data.irq = irq;
                data.softirq = softirq;
                data.steal = steal;

                save_to_buffer(data);
            }
        }
        
        fclose(fptr);

        t.tv_sec = 0;
        t.tv_nsec = 50000000L;
        nanosleep(&t, NULL);
    }
}
