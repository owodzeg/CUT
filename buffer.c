#include "buffer.h"
#include <stdio.h>

unsigned long old_cores;
unsigned long max_cores;
struct CoreData *cores = NULL;

void save_to_buffer(struct CoreData data)
{
    unsigned long core_id = data.core_id;

    old_cores = max_cores;

    if(core_id+1 > max_cores)
    max_cores = core_id+1;

    if(cores == NULL)
    {
        //printf("Initializing cores with size: %d\n", sizeof(struct CoreData));
        cores = malloc(sizeof(struct CoreData));

        if(!cores)
        {
            //printf("Allocation failed\n");
        }
        else
        {
            //printf("Success\n");
        }
    }
    else if(old_cores != max_cores)
    {
        //printf("Attempting to resize cores to size %d\n", sizeof(struct CoreData) * max_cores);
        struct CoreData *cores_new = realloc(cores, sizeof(struct CoreData) * max_cores);

        if(!cores_new)
        {
            //printf("Allocation failed\n");
        }
        else
        {
            //printf("Success\n");
            cores = cores_new;
        }
    }

    //after cores is resized, i can safely store more data
    cores[core_id] = data;
}

struct CoreData* load_from_buffer(void)
{
    return cores;
}

unsigned long get_num_cores(void)
{
    return max_cores;
}
