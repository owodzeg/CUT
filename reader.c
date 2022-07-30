#include "reader.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* read_proc(void* arg)
{
    for(;;)
    {
        FILE *fptr;
        char line[1024];

        char cpu[255];
        long int user=0, nice=0, system=0, idle=0, iowait=0, irq=0, softirq=0, steal=0;

        char head[3];
        int core = 0;

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
                core = atoi(cpu+3);
                printf("cpu: %d, user: %ld, nice: %ld, system: %ld, idle: %ld, iowait: %ld, irq: %ld, softirq: %ld, steal: %ld\n", core, user, nice, system, idle, iowait, irq, softirq, steal);
            
                struct CoreData data;
                data.core_id = core;
                data.user = user;
                data.nice = nice;
                data.system = system;
                data.idle = idle;
                data.iowait = iowait;
                data.irq = irq;
                data.softirq = softirq;
                data.steal = steal;
            }
        }
        
        fclose(fptr);

        sleep(1);
    }
}
