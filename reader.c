#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

void read_proc()
{
    FILE *fptr;
    char cpu[255];
    long int user=0, nice=0, system=0, idle=0;

    if((fptr = fopen("/proc/stat", "r")) == NULL)
    {
        printf("Unable to open file\n");
        return;
    }

    printf("File read successfully\n");

    while(fscanf(fptr,"%s %ld %ld %ld %ld", cpu, &user, &nice, &system, &idle) != EOF)
    {
        printf("cpu: %s, user: %ld, nice: %ld, system: %ld, idle: %ld\n", cpu, user, nice, system, idle);
    }
    
    fclose(fptr);
}
