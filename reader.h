// module for reading cpu data from /proc/stat and transporting it to a data buffer

extern int reader_running;
void* read_proc(void*);
void reader_exit(void);
