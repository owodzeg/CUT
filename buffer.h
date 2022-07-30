// module that'll be used for buffering oncoming data from reader, 
// so it can be received by analyzer in a neat queue

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

void add_to_queue(struct CoreData *core);
struct CoreData read_from_queue(void*);
