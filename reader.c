#define _GNU_SOURCE //define

#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_proc()
{
    FILE *fptr;
    char line[1024];

    char cpu[255];
    long int user=0, nice=0, system=0, idle=0, iowait=0, irq=0, softirq=0, steal=0;

    char head[3];

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
        printf("cpu: %s, user: %ld, nice: %ld, system: %ld, idle: %ld, iowait: %ld, irq: %ld, softirq: %ld, steal: %ld\n", cpu, user, nice, system, idle, iowait, irq, softirq, steal);
    }
    
    fclose(fptr);
}
