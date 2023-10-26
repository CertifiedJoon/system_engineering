pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&lock);
//critical seleciton
pthread_mutex_unlock(&lock);