/**
 * Process is created by 
 * 1. load code from hard disk drive
 * 2. allocate stack memory
 * 4. store pc, pid into pcb and place into process list
 * 3. allocate heap memory
 * 
 * Multi core scheduling
 * 1. single queue implementation -> cache affinity poor scalability
 * 2. multi queue implementation -> provides cache affinity. less concurrency issue. but load imbalance, solved by work stealing
 *  
 * pthread_create(&thread, NULL, routine, * value)
 * 
 * pthread_join(thread, **rvalue)
 */ 