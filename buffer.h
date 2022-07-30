// module that'll be used for buffering oncoming data from reader, 
// so it can be received by analyzer in a neat queue

#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

// contain the necessary data of a single core
// we differentiate cores so we can calculate per core % usage
struct CoreData
{
    int core_id;
    long int user;
    long int nice;
    long int system;
    long int idle;
    long int iowait;
    long int irq;
    long int softirq;
    long int steal;
};

// we keep track of the old core count so we don't do expensive reallocs all the time
extern int old_cores;
extern int max_cores;

// data storage
extern struct CoreData *cores;

void save_to_buffer(struct CoreData data);

#endif
