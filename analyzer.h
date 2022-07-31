// module for processing the oncoming data from buffer
#include "buffer.h"

extern struct CoreData* old_core_data;
extern struct CoreData* new_core_data;
extern double* averagesStored;

_Noreturn void* process_data(void*);
double* retrieve_data(void);
