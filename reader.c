#define _GNU_SOURCE //define

#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

void read_proc()
{
    FILE *fptr;
    char line[1024];

    char cpu[255];
    long int user=0, nice=0, system=0, idle=0;

    fptr = fopen("/proc/stat", "r");
    
    if(fptr == NULL)
    {
        //TODO: replace it with logger thread
        printf("Could not open file.\n");
    }

    while(fgets(line, 1024, fptr) != NULL) 
    {
        //printf("%s", line);
        sscanf(line, "%s %ld %ld %ld %ld", cpu, &user, &nice, &system, &idle);
        printf("cpu: %s, user: %ld, nice: %ld, system: %ld, idle: %ld\n", cpu, user, nice, system, idle);
    }
    
    fclose(fptr);
}
